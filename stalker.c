float zrstate[12];
float otherzrstate[12];
float target[3];
float midpoint[3];
int ID;
char state;
int canGoTo(float* aim){
    //|-x1 cross -x2|/|x2-x1|
    float first[3];
    float second[3];
    float numerator[3];
    float denominator[3];
    int i=0;
    float center[3];
    while (i<3){
        first[i]=-1*zrstate[i];
        second[i]=-1*aim[i];
        center[i]=0;
        i++;
    }
    mathVecCross(numerator,first,second);
    mathVecSubtract(denominator,second,first,3);
    float c = mathVecMagnitude(numerator,3)/mathVecMagnitude(denominator,3);
    if (c>0.35){
        DEBUG(("can go\n"));  
        DEBUG(("heading towards %f,%f,%f\n",target[0],target[1],target[2]));
    }
    return c>0.35;   
}
void goAround(float* c){
    if (! canGoTo(c)){
        api.setPositionTarget(midpoint);
    }else{
        api.setPositionTarget(c);
    }
    
}

void init(){
    api.getMyZRState(zrstate);
    state = 0;
    if (zrstate[1] > 0)
        ID = 0;
    else
        ID = 1;
    midpoint[0]=-0.6;
    midpoint[1]=0;
    midpoint[2]=0;
    
}

void loop(){
    api.getMyZRState(zrstate);
    api.getOtherZRState(otherzrstate);
    int i=0;
    while (i<3){
        target[i]=otherzrstate[i++];
    }
    goAround(target);
    if (game.getNextFlare() < 2 && game.getNextFlare() > -1) {
          game.turnOff(); 
          game.turnOn();
    }
    
    
}
