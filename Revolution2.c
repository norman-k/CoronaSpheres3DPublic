//Declare any variables shared between functions here

float counter;
float sphere_mass;

void init(){
    counter = 0;
    sphere_mass = 4.8f;
}

void loop(){
    // Revolution function parameters
    float center[3] = {1.0f, 0.0f, 0.0f};
    float axis[3] = {1.0f, 1.0f, 1.0f}; // Like an attitude. Orientation of revolution
    float radius = 3.0f; 
    float period = 5.0f;
    
    // v = 2*pi*r/T (Uniform circular motion)
    // w = 2*pi/T (Uniform circular motion) 

    float velocity_magnitude = (2 * 3.14159 * radius) / period;
    float rot_rate = (2 * 3.14159) / period;
    float accel_magnitude = velocity_magnitude * velocity_magnitude / radius;
    
    // Procedural stuff

    mathVecNormalize(axis, 3);
    
    float myPos;
    float myState;
    api.getMyZRState(myState);
    for (int i = 0; i < 3; i++) {
        myPos[i] = myState[i];
    }
    
    // Travel to starting radius
    float true_radius;
    mathVecSubtract(true_radius, myPos, center, 3);
    float true_radius_mag = mathVecMagnitude(true_radius, 3);
    
    while (abs(true_radius_mag - radius) > 0.5) {
        if (true_radius - radius > 0) {
            // Outside
            // Move in direction given by negation of true_radius
            api.setPositionVector(center);
        } else {
            // Inside 
            // Move in direction given by true_radius
            api.setVelocityVector(true_radius);
            
        }
    }
    
    DEBUG(("MADE IT"));
    
    
    
    

    
    
    
    // The cross product of a and r is v orientation. 
    // v magnitude given. 
    // v orientation from crossproduct. 
    
    
    


}
