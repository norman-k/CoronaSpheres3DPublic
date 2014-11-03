float collision_distance () {
	float s1[12];	//my SPHERE
	float s2[12];	//enemy SPHERE
	//s1 s2 prediction scheme for next second
	api.getOtherZRState(s2);
	api.getMyZRState(s1);
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
