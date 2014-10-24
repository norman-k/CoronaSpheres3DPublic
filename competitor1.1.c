float zrstate[12];
float POI[3];
float POI0[3];
float POI1[3];
float POI2[3];
float angle[3];
float target[3];
bool POIS[3];
int ID;
char state;

float getDist(float pt1[3], float pt2[3]) {
    return sqrtf(mathSquare(pt2[2] - pt1[2]) + mathSquare(pt2[1] - pt1[1]) + mathSquare(pt2[0] - pt1[0]));
}

void getPOI(float poi[3],float id) {
    game.getPOILoc(poi,id);
    angle[0] = -5*poi[0];
    angle[1] = -5*poi[1];
    angle[2] = -5*poi[2];
}

int getClosestPOI(float poi[3], float poi0[3], float poi1[3], float poi2[3], 
    bool pois[3], float state[12]) {
    game.getPOILoc(poi0,0);
    game.getPOILoc(poi1,1);
    game.getPOILoc(poi2,2);
    if (!pois[0])
        poi0[0] = 999;
    if (!pois[1])
        poi1[0] = 999;
    if (!pois[2])
        poi2[0] = 999;
    float d0,d1,d2;
    d0 = getDist(poi0,state);
    d1 = getDist(poi1,state);
    d2 = getDist(poi2,state);
    if (d0 < d1 && d0 < d2) {
        return 0; 
    }
    
    else if (d1 < d0 && d1 < d2) {
        return 1; 
    }
    
    else {
        return 2; 
    }
}

float angleBetween(float A[3], float B[3]) {
    
    float dotProduct = mathVecInner(A, B, 3);
    float magnitudeA = mathVecMagnitude(A, 3);
    float magnitudeB = mathVecMagnitude(B, 3);
    
    float angle = acos(dotProduct / (magnitudeA * magnitudeB));
    
    return angle; 
}

bool canTakePic
    (float POI[3], float state[12], float maxAngle, float maxDist, float minDist, int ID) {
    //to check for inner zone: maxAngle = 0.8, maxDist = 0.42, minDist = 0.31
    //to check for outer zone: maxAngle = 0.4, maxDist = 0.53, minDist = 0.42
    return (fabsf(angleBetween(state,POI)) < maxAngle &&
            game.alignLine(ID) &&
            (minDist < mathVecMagnitude(state,3) && 
            mathVecMagnitude(state,3) < maxDist));
}

void init(){
    POIS[0] = 1;
    POIS[1] = 1;
    POIS[2] = 0;
    api.getMyZRState(zrstate);
    state = 0;
    if (zrstate[1] > 0)
        ID = 0;
    else
        ID = 1;
    getPOI(POI,ID);
    target[0] = 1.8*POI[0];
    target[1] = 1.8*POI[1];
    target[2] = 1.8*POI[2];
}

void loop(){
    api.getMyZRState(zrstate);
    if (game.getNextFlare()%5 == 0) {
        DEBUG(("%i",game.getNextFlare()));
    }
    
    if (game.getNextFlare() < 4 && game.getNextFlare() > -1 && state != 2) {
        DEBUG(("Slowing!"));
        float zero[3] = {0.0f,0.0f,0.0f};//number 4 is a guess
        //zero[0] = 0;zero[1] = 0;zero[2] = 0;
        api.setVelocityTarget(zero);
    }
    
    if (game.getNextFlare() < 2 && game.getNextFlare() > -1) {
          game.turnOff(); 
          game.turnOn();
    }
    
    if (api.getTime()%60 < 1 && api.getTime() > 20) {
        target[0] = 3*POI[0];
        target[1] = 3*POI[1];
        target[2] = 3*POI[2];
        POIS[0] = 1;
        POIS[1] = 1;
        POIS[2] = 1;
        state = 2;
    }
    
    if (state == 0) {
        if (canTakePic(POI,zrstate,0.8,0.42,0.31,ID)) {
            game.takePic(ID);
            if (game.getNextFlare() < 17 && game.getNextFlare() > 8) {
                target[0] = 4*POI[0]; //numbers 17 and 8 are for old movement code
                target[1] = 4*POI[1];
                target[2] = 4*POI[2];
                POIS[ID] = 0;//LOGIC NOT FULLY SOUND
                state = 2;//WE WILL HAVE TO STORE WETHER YOU CAN TAKE INNER AND 
            }//WETHER YOU CAN TAKE OUTER. THIS MEANS CHANGING A LOT OF STUFF.
            else {
                target[0] = 2.5*POI[0];
                target[1] = 2.5*POI[1];
                target[2] = 2.5*POI[2];
                state = 1;
            }
        }
        else {
            api.setPositionTarget(target);
            api.setAttitudeTarget(angle);
        }
    }
    
    if (state == 1) {
       if (canTakePic(POI,zrstate,0.4,0.53,0.42,ID)) {
            game.takePic(ID);
            POIS[ID] = 0;
            target[0] = 3*POI[0];
            target[1] = 3*POI[1];
            target[2] = 3*POI[2];
            state = 2;
        }
        else {
            api.setPositionTarget(target);
            api.setAttitudeTarget(angle);
        } 
    }
    
    if (state == 2) {
        if (mathVecMagnitude(zrstate,3) > 0.53) {
            game.uploadPic();
            target[0] = 2.9*POI[0];
            target[1] = 2.9*POI[1];
            target[2] = 2.9*POI[2];
            state = 3;
        }
        api.setPositionTarget(target);
    }
    
    if (state == 3) {
        game.takePic(ID);
        if (api.getTime()%60 < 40) {
            ID = getClosestPOI(POI,POI0,POI1,POI2, POIS, zrstate);
            getPOI(POI,ID);
            float tmp[3];
            mathVecSubtract(tmp,zrstate,POI,3);
            if (mathVecMagnitude(tmp,3) <= 0.65) {
                target[0] = 1.8*POI[0];
                target[1] = 1.8*POI[1];
                target[2] = 1.8*POI[2];
                state = 0;
                api.setAttitudeTarget(angle);
            }
        }
        api.setPositionTarget(target);
    }
}
