//Updates:
//+ Movement code tweaked so it crashes a lot less (it seems) and behaves better with opponents
//+ Faster movement from long distances
//+Quicker logic in taking pictures
//Issues:
//-getTangentPoint() still sometimes breaks, 
//-Logic still not sound on whether or not to go for a second POI (only time taken into account, not distance or opponent.)
//-A few numbers are still off
//-STILL DOES NOT TAKE OTHER COMPETITOR INTO ACCOUNT
//Note: We might want to look into moving to the shadow zone if sphere is close enough
float zrstate[12];
float POI[3];
float POI0[3];
float POI1[3];
float POI2[3];
float angle[3];
float target[3];
bool POIS[3];
float midpoint[3];
int ID;
char state;
int INTERVALS;

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

bool clearPath(float target[3]) {
    float cross[3]; 
    float sub[3];
    float o = zrstate[0] * target[0] + zrstate[1] * target[1] + zrstate[2] * target[2];
    mathVecCross(cross,zrstate,target);
    mathVecSubtract(sub,zrstate,target,3);
    float d = mathVecMagnitude(cross,3) / mathVecMagnitude(sub,3);
    return (d > 0.33 || o > 0.1);//change o > _ to get difference tolerances
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

void moveTo(float target[3]){
    DEBUG(("\n"));
    if (clearPath(target)){ 
        DEBUG(("SET COURSE FOR (%f, %f, %f)\n",target[0],target[1],target[2]));
        goFast(target);
    } 
    else {
        float tmp[3];
        float tmp2[3];
        float sub[3];
        /*findTangPoint(tmp,zrstate,target,0.33);
        tmp[0] = tmp[0] * 1.2;
        tmp[1] = tmp[1] * 1.2;
        tmp[2] = tmp[2] * 1.2;
        if (tmp[0]) {//If this is null, this should be false, right?
            DEBUG(("TRUE%f",tmp[0]));
        }//if findTangPoint breaks, use goAround
        else {
            DEBUG(("FALSE%f",tmp[0]));
            DEBUG(("!!!FINDTANGPOINT BROKE!!!\n"));
            goAround(tmp,zrstate,target,0.33);
        }*/
        if (mathVecMagnitude(zrstate,3) <= 0.39)
            goAround(tmp,zrstate,target,0.33, 5);
        else
            findTangPoint(tmp,zrstate,target,0.33);
            
        //mathVecSubtract(sub,tmp,zrstate,3);
        //mathVecNormalize(sub,3);
        //mathVecAdd(tmp2,tmp,sub,3);
        DEBUG(("COLLISION COURSE -> NEW COURSE (%f, %f, %f)\n",tmp[0],tmp[1],tmp[2]));
        DEBUG(("COLLISION COURSE -> ACTUAL NEW VELOCITY (%f, %f, %f)\n",tmp2[0],tmp2[1],tmp2[2]));
        //api.setVelocityTarget(tmp2);
        goFast(tmp);
    }
}

//less optimized but not buggy code to use if findTangPoint breaks:
void goAround(float result[3], float point[3],float targt[3], float r, int n){
    mathVecAdd(result,point,targt,3);
    mathVecNormalize(result,3);
    result[0] = result[0]*r*1.2;
    result[1] = result[1]*r*1.2;
    result[2] = result[2]*r*1.2;
    if (clearPath(result) || n <= 0){
        return;
    }
    else {
        goAround(result, point, result, r, n-1);
    }
}

void findTangPoint(float result[3], float point[3],float targt[3], float r) {
    float DNorms[3];
    float targ[3];
    targ[0]=targt[0];targ[1]=targt[1];targ[2]=targt[2];
    float D = mathVecMagnitude(point,3);
    mathVecNormalize(point,3);
    mathVecNormalize(targ,3);
    mathVecSubtract(DNorms,targ,point,3);
    mathVecNormalize(DNorms,3);
    float alpha = acosf(r/D);
    float beta = angleBetween(point,DNorms);
    float theta = beta - alpha;
    float dist = sinf(alpha)/sinf(theta);
    DNorms[0] = DNorms[0]*dist;
    DNorms[1] = DNorms[1]*dist;
    DNorms[2] = DNorms[2]*dist;
    mathVecAdd(result,point,DNorms,3);
    mathVecNormalize(result,3);
    result[0] = result[0] * r * 1.2;
    result[1] = result[1] * r * 1.2;
    result[2] = result[2] * r * 1.2;
}

void goFast(float targt[3]) {
    float targ[3];
    float mag;
    mathVecSubtract(targ,targt,zrstate,3);
    mag = mathVecMagnitude(targ,3);
    if (mag <= 0.2) {
        api.setPositionTarget(targt);
    }
    else {
        targ[0] = targ[0] * mag * mag * 40;
        targ[1] = targ[1] * mag * mag * 40;
        targ[2] = targ[2] * mag * mag * 40;
        mathVecAdd(targ,targt,targ,3);
        DEBUG(("ACTUAL TARGET: %f,%f,%f",targ[0],targ[1],targ[2]));
        api.setPositionTarget(targ);
    }
    api.setPositionTarget(targt);
}

void init(){
    INTERVALS=60;
    POIS[0] = 1;
    POIS[1] = 1;
    POIS[2] = 0;
    api.getMyZRState(zrstate);
    state = 0;
    ID = getClosestPOI(POI,POI0,POI1,POI2, POIS, zrstate);
    getPOI(POI,ID);
    target[0] = 1.9*POI[0];
    target[1] = 1.9*POI[1];
    target[2] = 1.9*POI[2];
}

void loop(){
    api.getMyZRState(zrstate);
    
    if (game.getNextFlare()%5 == 0) {
        DEBUG(("%i",game.getNextFlare()));
    }
    
    if (game.getNextFlare() < 4 && game.getNextFlare() > -1 && state != 2) {
        DEBUG(("Slowing!"));
        float zero[3] = {0.0f,0.0f,0.0f};//number 4 is a guess
        api.setVelocityTarget(zero);
    }
    
    if (game.getNextFlare() < 2 && game.getNextFlare() > -1) {
          game.turnOff(); 
          game.turnOn();
    }
    
    if (api.getTime()%INTERVALS < 1 && api.getTime() > 20) {
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
            if (game.getNextFlare() > 8 || game.getNextFlare() < 0) {
                game.takePic(ID);
                if (game.getNextFlare() < 17 && game.getNextFlare() > 0) {
                    target[0] = 4*POI[0]; 
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
        }
        else {
            moveTo(target);
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
            moveTo(target);
            api.setAttitudeTarget(angle);
        } 
    }
    
    if (state == 2) {
        if (mathVecMagnitude(zrstate,3) > 0.53) {
            game.uploadPic();
            target[0] = 2.2*POI[0];
            target[1] = 2.2*POI[1];
            target[2] = 2.2*POI[2];
            state = 3;
        }
        moveTo(target);
    }
    
    if (state == 3) {
        game.takePic(ID);
        if (api.getTime()%INTERVALS < (INTERVALS-20)) {//GUESSWORK
            ID = getClosestPOI(POI,POI0,POI1,POI2, POIS, zrstate);
            getPOI(POI,ID);
            float tmp[3];
            mathVecSubtract(tmp,zrstate,POI,3);
            if (mathVecMagnitude(tmp,3) <= 0.65) {
                target[0] = 1.9*POI[0];
                target[1] = 1.9*POI[1];
                target[2] = 1.9*POI[2];
                state = 0;
                api.setAttitudeTarget(angle);
            }
        }
        moveTo(target);
    }
}
         
