/* Michael Jin
	DEMO - consistent ~24 score
*/

//Declare any variables shared between functions here
float target[3], rotation[3], srotation[3], vector_velocity[3];
float POIT[3];				//point of interest target
float max_distance, max_speed;
int time, cam_time, status, traveled, offTime, run, nil, gotoOther, ttime;

void init(){
	//This function is called once when your code is first loaded.

	//IMPORTANT: make sure to set any variables that need an initial value.
	//Do not assume variables will be set to 0 automatically!
	//float make_target[3] = {0, .15, 0};
	
	//memcpy(target, make_target, 3*sizeof(float));
	
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
	
}

void loop(){
	//This function is called once per second.  Use it to control the satellite.
	ZRState myState, oState;
	float zero_speed[3] = {0,0,0};
	float distance_left;//, speed;
	
	api.getMyZRState(myState);
	api.getOtherZRState(oState);
	float direction_vector[3] = {-myState[0],-myState[1],-myState[2]};
	api.setAttitudeTarget(direction_vector);
	if (true){
		gotoOther = getPOIID(myState);
		if (collision_distance(myState,oState) < .3) {
			game.getPOILoc(target, openPOI(myState, gotoOther));	//supply zr state as parameter
		}
		else {
			game.getPOILoc(target, getPOIID(myState));	//supply zr state as parameter
		}
		if (target[0] != POIT[0] && target[1] != POIT[1] && target[2] != POIT[2]) {
			POIT[0] = target[0];
			POIT[1] = target[1];
			POIT[2] = target[2];
			if (game.getMemoryFilled() < game.getMemorySize()){
				status = 1;
			}
			
			DEBUG(("point of interest changed!\n"));
		}
		float spz = 1.75;	//picture zone multiplier
		target[0] *= spz;
		target[1] *= spz;
		target[2] *= spz;
		DEBUG(("time: %i | status: %i | px: %lf | py: %lf | pz: %lf\n", time, status, POIT[0], POIT[1], POIT[2]));
		DEBUG(("time: %i | status: %i | x: %lf | y: %lf | z: %lf\n", time, status, myState[0], myState[1], myState[2]));
	}
	if (game.getNextFlare()>-1 && game.getNextFlare() < 6) {
		api.setVelocityTarget(zero_speed);
		if (game.getNextFlare() == 1 || game.getNextFlare() == 0) {
				game.turnOff();
				game.turnOn();
				status = 1;
				//++offTime;
		}
		//game.turnOff(); 
	}
	else {
		/*if (offTime > 0) {
			if (offTime > 3) {
				offTime = 0;
			}
			game.turnOn();
			++offTime;
		}*/
	//game.turnOn();
	
	if (status == 1) {
		distance_left = delta_distance(myState,target);
		if (distance_left < max_distance) {
			if (cam_time == 0 && game.getMemoryFilled() < game.getMemorySize()) {
				DEBUG(("Taking pictures!"));
				++cam_time;
			}
			else if (game.getMemorySize() > 0 && (game.getNextFlare() == -1 || game.getNextFlare() > 20)) {
				DEBUG(("Memory filled! Next flare: %i\n", game.getNextFlare()));
				status = 2;
				ttime = 0;
				cam_time = 0;
			}
			else if (cam_time < 4){
				++cam_time;
			}
			else if (cam_time > 3) {
				cam_time = 0;
			}
			DEBUG(("picture count: %i | total size: %i\n", game.getMemoryFilled(), game.getMemorySize()));
		}
	}
	if (status == 2) {
		game.takePic(getPOIID(myState));
		target[0] = 2.8 * POIT[0];
		target[1] = 2.8 * POIT[1];
		target[2] = 2.8 * POIT[2];
		float r1 = sqrtf((target[0] * target[0]) + (target[1] * target[1]) + (target[2] * target[2]));
		float r0 = sqrtf((myState[0] * myState[0]) + (myState[1] * myState[1]) + (myState[2] * myState[2]));
		//distance_left = delta_distance(myState,target);
		//if ((distance_left < max_distance)) {
		if (abs(r1 - r0) < max_distance) {
			game.uploadPic();
			DEBUG(("Took %i seconds to reach danger zone!\n", ttime));
		}
		if (ttime > 25) {
			game.turnOn();
			status = 1;
		}
		++ttime;
	}
	api.setPositionTarget(target);
	}
	DEBUG(("collsion distance: %lf\n", collision_distance(myState, oState)));
	DEBUG(("time: %i | status: %i | tx: %lf | ty: %lf | tz: %lf\n", time, status, target[0], target[1], target[2]));
	time += 1;
	
}

float collision_distance (float *s1, float *s2) {
	//float s1[12]	//my SPHERE
	//float s2[12]	//enemy SPHERE
	//s1 s2 prediction scheme for next second
	s1[0] = s1[0] + s1[3];  //x position next second
	s1[1] = s1[1] + s1[4];  //y position next second
	s1[2] = s1[2] + s1[5];  //z position next second
	s2[0] = s2[0] + s2[3];  //x position next second
	s2[1] = s2[1] + s2[4];  //y position next second
	s2[2] = s2[2] + s2[5];  //z position next second
	float dist = 0.0f;
	dist = dist + ((s2[0]-s1[0]) * (s2[0]-s1[0]));
	dist = dist + ((s2[1]-s1[1]) * (s2[1]-s1[1]));
	dist = dist + ((s2[2]-s1[2]) * (s2[2]-s1[2]));
	dist = sqrtf(dist);	//norm of distance vector
	return dist;
}

int openPOI(float *pos, int i) {
    int ans = 0; //maxid
    float POI[3];
    float tempd=0;
    //visible POI are assigned to id 0,1,2
    for (int id = 0; id<3; id++){
		game.getPOILoc(POI,id);
        if (id == 0){
			if (id != i) {
				tempd = delta_distance(POI, pos);
			}
			else {
				game.getPOILoc(POI,1);
				tempd = delta_distance(POI, pos);
				ans = 1;
			}
        }   
        else {
			if (id != i) {
            	if (delta_distance(POI, pos) < tempd) {
					tempd = delta_distance(POI, pos);
					ans = id;
				}
            }
        }
    }
     //~.35/.2 (radius of the asteroid)-> magnitude for target position for safe pic taking
    return ans;
}

int getPOIID(float *pos){		//given zr state
    int ans = 0; //maxid
    float POI[3];
    float tempd=0;
    //visible POI are assigned to id 0,1,2
    for (int id = 0; id<3; id++){
		game.getPOILoc(POI,id);
        if (id == 0){
            tempd = delta_distance(POI, pos);
        }   
        else {
            if (delta_distance(POI, pos) < tempd) {
                tempd = delta_distance(POI, pos);
                ans = id;
            }
        }
    }
     //~.35/.2 (radius of the asteroid)-> magnitude for target position for safe pic taking
    return ans;
}

float delta_distance(float *pos1, float *pos2) {
	float d2 = 0;
	for (int i = 0; i < 3; ++i)
		d2 = d2 + (pos1[i]-pos2[i])*(pos1[i]-pos2[i]);
	return sqrt(d2);
}
