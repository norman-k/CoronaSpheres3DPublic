/* Michael Jin
	DEMO - take memory pack
*/

//Declare any variables shared between functions here
float target[3], rotation[3], srotation[3], vector_velocity[3];
float POIT[3];				//point of interest target
float max_distance, max_speed;
int time, cam_time, status, traveled, offTime, run, nil, gotoOther, ttime;
int mid;

void init(){
	//This function is called once when your code is first loaded.

	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
	//float make_target[3] = {0, .15, 0};
	
	//memcpy(target, make_target, 3*sizeof(float));
	mid = 0;
	max_distance = 0.03;
	max_speed = 0.01;
	traveled = 0;
	nil = 0;
	gotoOther = 0;
	for (int count = 0; count < 3; count++)
	{
		srotation[count] = 0;	//brake rotation
		if (count != 1)
		{
		vector_velocity[count] = 0;
		}
		else
		{
			vector_velocity[1] = .04;
		}
		if (count != 2)
		{
			rotation[count] = 0;
		}
	}
	POIT[0] = 0;
	POIT[1] = 0;
	POIT[2] = 0;
	time = 0;
	cam_time = 0;
	offTime = 0;
	ttime = 0;
	//item = 0;
	status = 1;
	    target[0] = -.5;
		target[1] = -.6;
		target[2] = 0;
}

void loop(){
	//This function is called once per second.  Use it to control the satellite.
	ZRState myState, oState;
	float zero_speed[3] = {0,0,0};
	float distance_left;//, speed;
	
	api.getMyZRState(myState);
	api.getOtherZRState(oState);
	if (time == 0) {
	    if (myState[1] > 0) {
	        target[1] *= -1;
	        mid = 1;
	    }
	}
		DEBUG(("time: %i | status: %i | x: %lf | y: %lf | z: %lf\n", time, status, myState[0], myState[1], myState[2]));

	distance_left = delta_distance(myState,target);
		if (distance_left < max_distance && !game.hasMemoryPack(0, mid)) {
			
		rotation[0] = 0;
		rotation[1] = 0;
		rotation[2] = 0.218166156499291196528;
		
		
		api.setAttRateTarget(rotation);
		}
		else if (game.hasMemoryPack(0, mid)) {
		    target[0] = 0;
		}
	api.setPositionTarget(target);
	DEBUG(("time: %i | status: %i | tx: %lf | ty: %lf | tz: %lf\n", time, status, target[0], target[1], target[2]));
	time += 1;
	
}

float delta_distance(float *pos1, float *pos2) {
	float d2 = 0;
	for (int i = 0; i < 3; ++i)
		d2 = d2 + (pos1[i]-pos2[i])*(pos1[i]-pos2[i]);
	return sqrt(d2);
}
