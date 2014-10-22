//Declare any variables shared between functions here

float initRadius;
float myState[12];
float myPos[3];
float relPos[3];
float relTargetPos[3];
float targetPos[3];
float targetAtt[3];
float counter;
float currRadius;
float thresholdValue;
float deltaRad;

void init(){
    counter = 0;
    thresholdValue = 0.5f;
}

void loop(){
    float center[3] = {1.0f, 0.0f, 0.0f};
    float axis[3] = {1.0f, 1.0f, 1.0f};
    
    mathVecNormalize(axis, 3);
    
    api.getMyZRState(myState);
    for (int i = 0; i < 3; i++) {
        myPos[i] = myState[i];
    }
    
    mathVecSubtract(relPos, myPos, center, 3);
    
    currRadius = mathVecMagnitude(relPos, 3);
    
    if (counter > 0) {
        DEBUG(("%f", currRadius));
    }
    
    if (counter == 0) {
        initRadius = currRadius;
        DEBUG(("%f", initRadius));
        DEBUG(("- ^ Init -"));
    }
    
    mathVecCross(relTargetPos, relPos, axis); 
    mathVecAdd(targetPos, center, relTargetPos, 3);
    
    mathVecSubtract(targetAtt, center, myPos, 3);
    mathVecNormalize(targetAtt, 3);
    
    // Actions
    
    deltaRad = (abs(currRadius - initRadius));
    
    // The aim is to keep a constant radius for 
    // uniform circular motion (Ideal).
    
    // It's apparant that the radius is decreasing. 
    
    // It looks like after a lot of time, the radius stabilizes. 
    // Need to let function have a radius argument 
    // And get into the position really fast
    
    if (deltaRad > thresholdValue) { // Radius is getting too small
        // Here should be something that maintains / increases radius
    } else {
        api.setPositionTarget(targetPos);
    }
    
    api.setAttitudeTarget(targetAtt);
    
    counter++;
}
