//Addition to Aaron's Competitor1. Basic detection and avoidance of asteroid collision.
//Relies upon FindTangPoint to correct movement when it is not otherwise possible.

//Keep in mind findTangPoint is still buggy. Once we have a master movement function, 
//we should put it and its helper functions in their own file so people can easily put them in newer competitors.
//-Aaron
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
            if (game.getNextFlare() < 17 && game.getNextFlare() > -1) {
                target[0] = 4*POI[0];
                target[1] = 4*POI[1];
                target[2] = 4*POI[2];
                state = 2;
            }
            else {
                target[0] = 2.5*POI[0];
                target[1] = 2.5*POI[1];
                target[2] = 2.5*POI[2];
                state = 1;
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
            target[0] = 2.9*POI[0];
            target[1] = 2.9*POI[1];
            target[2] = 2.9*POI[2];
            state = 3;
        }
        moveTo(target);
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
        if (game.getNextFlare() == 2) {
            game.turnOff();
            game.turnOn();
        }
    }
}

void moveTo(float target[3]){
    float cross[3]; 
    float sub[3];
    float o = zrstate[0] * target[0] + zrstate[1] * target[1] + zrstate[2] * target[2];
    mathVecCross(cross,zrstate,target);
    mathVecSubtract(sub,zrstate,target,3);
    float d = mathVecMagnitude(cross,3) / mathVecMagnitude(sub,3);
    if (d > 0.33 || o > 0){
        api.setPositionTarget(target);
    }else{
        float tmp[3];
        float loc[3];
        loc[0] = zrstate[0];
        loc[1] = zrstate[1];
        loc[2] = zrstate[2];
        //mathVecAdd(tmp,target,?,3);
        findTangPoint(tmp,loc,target,0.33);
        DEBUG(("COLLISION COURSE -> NEW COURSE (%f, %f, %f)\n",tmp[0],tmp[1],tmp[2]));
        api.setPositionTarget(tmp);
    }
}
void findTangPoint(float result[3], float point[3],float targ[3], float r) {
    float DNorms[3];
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
    result[0] = result[0] * r;
    result[1] = result[1] * r;
    result[2] = result[2] * r;
}
