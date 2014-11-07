#define EPSILON 0.1

float target[3];
float myState[12];
float myPos[3];
float myVelocity[3];
float vectorBetween[3];
float distance;

void init(){
  target[0]=0.8;
  target[1]=0.0;
  target[2]=0.0;
}

//Overlaunches by 0.01 - 0.04 (at most)                                         
void moveFast(float pt1[3], float pt2[3]){
  float velocity=mathVecMagnitude(myVelocity,3);
  float threshold=0.5*distance + velocity*velocity/(0.0086*4);
  if (distance>threshold){
    DEBUG(("using VelocityTarget"));
    DEBUG(("threshold:%f \n distance:%f",threshold,distance));
    api.setVelocityTarget(vectorBetween);
  }
  else{
    DEBUG(("using PositionTarget"));
    api.setPositionTarget(pt2);
  }
}

void loop(){
  api.getMyZRState(myState);
  for (int i=0; i<3; i++){
    myPos[i]=myState[i];
    myVelocity[i]=myState[i+3];
  }
  mathVecSubtract(vectorBetween,target,myPos,3);
  distance = mathVecMagnitude(vectorBetween,3);
  
  moveFast(myPos,target);
}
