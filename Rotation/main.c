// Added by Elvin

//Declare any variables shared between functions here

int counter;
// Seconds elapsed 

float position[3];
float axis[3];

float deltaAtt;
float startingAtt[3];


void init(){
	//This function is called once when your code is first loaded.

	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
	counter = 0;
	
    position[0] = 1.0f;
    position[1] = 1.0f;
    position[2] = 1.0f;
    axis[0] = 0.0f;
    axis[1] = 1.0f;
    axis[2] = 0.0f;
}

void loop(){
    
    // BEGIN: Misc.
	//This function is called once per second.  Use it to control the satellite.
    //goTo(position);
    //DEBUG(("%i", counter));
    // END: Misc.
    
    // BEGIN: Get starting attitude
    float myState[12];
    api.getMyZRState(myState);
    for (int i = 0; i < 3; i++) {
        startingAtt[i] = myState[i + 6];
    }

    // END: Get starting attitude
    
    // BEGIN: Function calls
    rotate(true, axis, 3.14159);
    // END: Function calls
    
}
