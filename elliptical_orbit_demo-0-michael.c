//Declare any variables shared between functions here
float target[3], rotation[3], vector_velocity[3], comet[6], cv[3];
float max_distance, max_speed, grav;
float distance_left, speed, xy[3];
int time, stage, side;

void init(){
	//This function is called once when your code is first loaded.

	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
	grav = 0.0;
	max_distance = 0.03;
	max_speed = 0.01;
	rotation[0] = 0.218166156499291196528;
	rotation[1] = 0;
	rotation[2] = 0;
	//vector_velocity[0] = -2.5;
	vector_velocity[0] = -1.5;
	vector_velocity[1] = -1.5;
	//vector_velocity[2] = 0;
	vector_velocity[2] = 0;
	target[0] = 0;
	target[1] = 0;
	target[2] = 0;
	comet[2] = 0;
	comet[1] = 0.8;
	comet[0] = 0;
	stage = 1;
	time = 0;
	side = 1;
}

void loop(){
    ZRState myState, oState;
	float zero_speed[3] = {0,0,0};
	//This function is called once per second.  Use it to control the satellite.
	api.getMyZRState(myState);
	api.getOtherZRState(oState);
	xy[0] = myState[0];
	xy[1] = myState[1];
	xy[2] = myState[2];
	float norm = 1/sqrtf((myState[0]*myState[0])+(myState[1]*myState[1])+(myState[2]+myState[2]));		//the norm of the velocity vector in 3D
	vector_velocity[0] = (-myState[1]*norm*.66) - (myState[0]*norm*(.0+grav));		//clockwise direction with implied gravity
	vector_velocity[1] = (myState[0]*norm*.66) - (myState[1]*norm*(.0+grav));
	
	distance_left = delta_distance(myState,target);
	speed = delta_distance(myState+3,zero_speed);
	if (stage == 1) {
	    api.setVelocityTarget(vector_velocity);
	    api.setAttitudeTarget(xy);  //location of asteroid? origin - current position
	    grav += .22;
	}
	api.setAttitudeTarget(xy);
	time++;
}

float delta_distance(float *pos1, float *pos2) {
	float d2 = 0;
	for (int i = 0; i < 3; ++i)
		d2 = d2 + (pos1[i]-pos2[i])*(pos1[i]-pos2[i]);
	return sqrtf(d2);
}
