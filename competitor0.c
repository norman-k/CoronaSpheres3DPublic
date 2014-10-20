//includes the following functions: angleBetween,canTakePic,getPOI
//Does not account for opponent, only minor things done to account for flares
//KNOWN BUGS: After the first two pictures are taken, the picture in the outer zone is not taken sucessfully.
float zrstate[12];
float POI[3];
float angle[3];
float target[3];
int ID;
char state;

void getPOI() {
    game.getPOILoc(POI,ID);
    angle[0] = -5*POI[0];
    angle[1] = -5*POI[1];
    angle[2] = -5*POI[2];
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
    api.getMyZRState(zrstate);
    state = 0;
    if (zrstate[1] > 0)
        ID = 0;
    else
        ID = 1;
    getPOI();
    target[0] = 1.8*POI[0];
    target[1] = 1.8*POI[1];
    target[2] = 1.8*POI[2];
}

void loop(){
    api.getMyZRState(zrstate);
    if (api.getTime()%60 == 2 && api.getTime() > 60) {
        target[0] = -0.6;
        target[1] = 0;
        target[2] = 0;
        state = 2;
    }
    if (state == 0) {
        if (canTakePic(POI,zrstate,0.8,0.42,0.31,ID)) {
            game.takePic(ID);
            target[0] = 2.5*POI[0];
            target[1] = 2.5*POI[1];
            target[2] = 2.5*POI[2];
            state = 1;
        }
        else {
            api.setPositionTarget(target);
            api.setAttitudeTarget(angle);
        }
    }
    
    if (state == 1) {
       if (canTakePic(POI,zrstate,0.4,0.53,0.42,ID)) {
            game.takePic(ID);
            target[0] = -0.6;
            target[1] = 0;
            target[2] = 0;
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
            state = 3;
        }
        api.setPositionTarget(target);
    }
    
    if (state == 3) {
        game.takePic(ID);
        if (api.getTime()%60 < 30) {
            getPOI();
            target[0] = 1.8*POI[0];
            target[1] = 1.8*POI[1];
            target[2] = 1.8*POI[2];
            state = 0;
            api.setAttitudeTarget(angle);
        }
        api.setPositionTarget(target);
        if (game.getNextFlare() == 2) {
            game.turnOff();
            game.turnOn();
        }
    }
}
