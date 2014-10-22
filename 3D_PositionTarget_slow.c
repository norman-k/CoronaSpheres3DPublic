/*Lise's attempt at mimicing top player's strat
    with Michael's  debugging
Things to note:
1. this uses position target... therefore it is slower-> INCREDIBLY SLOWER... :(
    LOW SCORE.....
2. does not account for asteroid collision or opponent collision
*/
char status;
float POI[3];
float zrstate[12];
float zero[3];
float target[3];
float attitude[3];
float vel[3];
float ang;
int pid;
int sec;

float pie;
//float sec;
void init() {
    api.getMyZRState(zrstate);
    pie = 3.14159265359;
    POI[0] = POI[1]=POI[2] =0;
    status = 0;
    zero[0]=zero[1]=zero[2]=0;
    sec =0;
}
int getPOI(){
    int ans = 0; //maxid
    //float *POI = malloc(3 * sizeof(float));
    float POI[3];
    float tempd=0;
    //visible POI are assigned to id 0,1,2
    for (int id = 0; id<3; id++){
        game.getPOILoc(POI,id);
        if (id == 0){
            tempd = dist(POI, zrstate);
        }   
        else {
            if (dist(POI,zrstate) < tempd) {
                tempd = dist(POI,zrstate);
                ans = id;
            }
        }
    }
     //From Michael Jin: ~.35/.2 (radius of the asteroid)-> magnitude for target position for safe pic taking
    //DEBUG(("initial ang: %f",ang));
    return ans;
}

void loop() {
    api.getMyZRState(zrstate);
    DEBUG(("status: %d, flare:%d\n",status,game.getNextFlare()));
    //int mem = game.getMemorySize();
    if (game.getNextFlare()>-1 && game.getNextFlare() < 4) {
        //causes some crashes....
        //api.setVelocityTarget(zero);
        /*for (int i =3; i < 6; i++){
            vel[i]= -zrstate[i];
        }*/
        //api.setVelocityTarget(vel);
        game.turnOff(); 
        DEBUG(("%d, turned off\n", status));
        sec = 0;
    }
    else{
        if (sec ==0){
            game.turnOn();
        }
        while(sec <6){
            game.turnOn();
            api.setVelocityTarget(zero);
            api.setForces(zero);
            api.setAttitudeTarget(zero);
            api.setPositionTarget(target);
            sec++;
        }
        
        if (status == 0){
            pid=getPOI();
            game.getPOILoc(POI,pid);
            target[0] = 1.75*POI[0];
            target[1] = 1.75*POI[1];
            target[2] = 1.75*POI[2];
            api.setPositionTarget(target);
            attitude[0] = -zrstate[0];
            attitude[1] = -zrstate[1];
            attitude[2] = -zrstate[2];
            //attitude[0] = 0.218166156499291196528;
            /*for (int i =0; i < 3; i++)
                attitude[i] = -4*POI[i];
                */
            api.setAttitudeTarget(attitude);
            DEBUG(("distance: %.4f\n",dist(zrstate,target) ));
            if (dist(zrstate,target)<.01){
                status = 3;
            }
    }
    if(status == 2){
        DEBUG(("STATUS 2"));
        if (game.getMemoryFilled() == 0){
            DEBUG(("back to start"));
            status = 0;
        }
        else{
                target[0] = 2.9*POI[0];
                target[1] = 2.9*POI[1];
                target[2] = 2.9*POI[2];
            if (dist(zrstate,target)<.04){
                    DEBUG(("uploading PIC\n"));
                    game.uploadPic();
                }
             //api.setPositionTarget(target);  
        }
    }
    if (status == 3) {
        if (game.getMemoryFilled() >=1){
            DEBUG(("Succeeded in taking picture\n"));
            status = 2;
        }else{
            pid=getPOI();
            game.getPOILoc(POI,pid);
            target[0] = 1.75*POI[0];
            target[1] = 1.75*POI[1];
            target[2] = 1.75*POI[2];
            api.setPositionTarget(target);
            attitude[0] = -zrstate[0];
            attitude[1] = -zrstate[1];
            attitude[2] = -zrstate[2];
            api.setAttitudeTarget(attitude);
            
            if (dist(zrstate,target)<.03){
                game.takePic(pid);
                DEBUG(("Taking Picture\n"));
            }
            
        }
       
    }
        api.setPositionTarget(target);
    
    }
}
float checkFlare(){
    if (game.getNextFlare() > 1 && game.getNextFlare()%5 == 0)
        DEBUG(("NextFlare->%i",game.getNextFlare()));
    if (game.getNextFlare() > 0 && game.getNextFlare() < 4) {
        game.turnOff(); 
    }
    else
        game.turnOn();
        
    return game.getNextFlare();
}

float dist(float *pos1, float *pos2) {
    float d2 = 0;
    for (int i = 0; i < 3; ++i)
    d2 = d2 + (pos1[i]-pos2[i])*(pos1[i]-pos2[i]);
    return sqrtf(d2);
}