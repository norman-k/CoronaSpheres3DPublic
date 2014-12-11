int timeLeft;   //.5 * (norm of distance vector/.1)
int mv_state;   //0 no velocity; 1 negative velocity; 2 positive velocity
int timeSince;  //wait 15 seconds before velocity target is prioritized

void moveTo(float* t, float* p){
    float vel[3];
    vel[0] = t[0]-p[0];
    vel[1] = t[1]-p[1];
    vel[2] = t[2]-p[2];
    float n = mathVecMagnitude(vel,3);
    if (mv_state == 0) {
        if (n > .2f && timeSince > 15) {
            mv_state = 1;
            timeLeft = 4;
            timeSince = 0;
        }
        api.setPositionTarget(target);
    }
    if (mv_state == 1) {
        if (timeLeft == 0) {
            mv_state = 0;
        }
        vel[0] *= 1.5/n;
        vel[1] *= 1.5/n;
        vel[2] *= 1.5/n;
        api.setVelocityTarget(vel);
    }
    if (timeLeft) {
        --timeLeft;
    }
    ++timeSince;
}