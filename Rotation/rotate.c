// Added by Elvin

// Generalized Tilted Axis Rotation

// Desired direction of rotation:
// True = CCW (+)
// False = CW (-)

// Axis: does not need to be normalized 

// Degrees specifies absolute value of angular displacement 
// (The CCW boolean lets you choose which "path" to take to get there)

void rotate(bool ccw, float axis[3], float deg) {
    // Get current attitude
    float myState[12];
    api.getMyZRState(myState);
    float myAtt[3];
    for (int i = 0; i < 3; i++) {
        myAtt[i] = myState[i + 6];
    }
    
    // Delta Attitude (startingAtt is global)
    deltaAtt = angleBetweenAtt(startingAtt, myAtt);
        
    while (deltaAtt < deg) {
        float myState[12];
        api.getMyZRState(myState);
    
        float myAtt[3];
        
        for (int i = 0; i < 3; i++) {
            myAtt[i] = myState[i + 6];
        }
        
        mathVecNormalize(axis, 3);
        
        float perpAttitude[3];
        
        if (ccw) {
            mathVecCross(perpAttitude, myAtt, axis);
        } else {
            // Cross product is anticommutative
            // By using the other perpendicular vector we can reverse 
            // the rotation
            mathVecCross(perpAttitude, axis, myAtt);
        }
        
        mathVecNormalize(perpAttitude, 3);
        
        api.setAttitudeTarget(perpAttitude);
        deltaAtt = angleBetweenAtt(startingAtt, myAtt);
        // Stabilization and parameters have not yet been coded/included
        // Another issue is time to slow down (accelerate/deaccelerate) to switch to another
        // -----------------------------
        
    }
    
}