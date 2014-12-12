/*
	Michael Jin - Predict future location with same magnitude from origin
*/

void rotVec (float* t) {        //must set x axis negative if you start as red
    float quat[4] = {cosf(.3), sinf(.3), 0, 0};     //.6 radian difference from now
    if (t[1] < 0) {            //start as red sphere
        quat[1] = -quat[1];
    }
	t[0] = ((1 - 2*quat[2]*quat[2] -2*quat[3]*quat[3])*t[0] + (2*quat[1]*quat[2] + 2*quat[0]*quat[3])*t[1] + (2*quat[1]*quat[3]-2*quat[0]*quat[2])*t[2]);
    t[1] = ((2*quat[1]*quat[2] - 2*quat[0]*quat[3])*t[0] + (1 - 2*quat[1]*quat[1] - 2*quat[3]*quat[3])*t[1] + (2*quat[2]*quat[3] + 2*quat[0]*quat[1])*t[2]);
    t[2] = ((2*quat[1]*quat[3] + 2*quat[0]*quat[2])*t[0] + (2*quat[2]*quat[3] - 2*quat[0]*quat[1])*t[1] + (1 - 2*quat[1]*quat[1] - 2*quat[2]*quat[2])*t[2]);
}