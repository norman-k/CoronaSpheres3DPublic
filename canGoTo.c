
int canGoTo(){
    //|-x1 cross -x2|/|x2-x1|
    float first[3];
    float second[3];
    float ans[3];
    float denominator[3];
    int i=0;
    while (i<3){
        first[i]=-1*zrstate[i];
        second[i]=-1*target[i];
    }
    mathVecCross(ans,first,second);
    mathVecSubtract(denominator,second,first,3);
    float c = mathVecMagnitude(ans,3)/mathVecMagnitude(denominator,3);
    
    return c>0.35;   
}
