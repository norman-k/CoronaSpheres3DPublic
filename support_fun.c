/* This file just contains support functions that might be useful for later on. 
shadDist computes the shortest distance from the Sphere to the shadow zone 
ptCircleDist computes the distance from a point to a circle, possibly to find the distance from the sphere to some zone. 
willCollide shows if sphere is dangerously close to asteroid */

#define AST_RAD 0.2; //radius of asteroid 
#define EPSILON 0.2;
float AST_CENTER[3]; //center of asteroid
float ltf_corner[3]; //top left forward corner 
float lbf_corner[3]; //bottom left forward corner
float ltb_corner[3]; //top left back corner 
float lbb_corner[3]; //bottom left back corner
float rtf_corner[3]; //top right forward corner
float rbf_corner[3]; //bottom right forward corner
float rtb_corner[3]; //right top back corner
float rbb_corner[3]; //right bottom back corner
void init() {
    float AST_CENTER_cpy[3] = {0.0,0.0,0.0};
    float ltf_corner_cpy[3] = {0.0,-0.2,0.2};
    float lbf_corner_cpy[3] = {0.0,-0.2,-0.2};
    float ltb_corner_cpy[3] = {0.64,-0.2,0.2}; 
    float lbb_corner_cpy[3] = {0.64,-0.2,-0.2};
    float rtf_corner_cpy[3] = {0.0,0.2, 0.2};
    float rbf_corner_cpy[3] = {0.0,0.2,-0.2};
    float rtb_corner_cpy[3] = {0.64,-0.2,0.2};
    float rbb_corner_cpy[3] = {0.63,-0.2,-0.2};
    
    memcpy(AST_CENTER, AST_CENTER_cpy, sizeof(float) * 3);
    memcpy(ltf_corner, ltf_corner_cpy, sizeof(float) * 3);
    memcpy(lbf_corner, lbf_corner_cpy, sizeof(float) * 3);
    memcpy(ltb_corner, ltb_corner_cpy, sizeof(float) * 3);
    memcpy(lbb_corner, lbb_corner_cpy, sizeof(float) * 3);
    memcpy(rtf_corner, rtf_corner_cpy, sizeof(float) * 3);
    memcpy(rbf_corner, rbf_corner_cpy, sizeof(float) * 3);
    memcpy(rtb_corner, rtb_corner_cpy, sizeof(float) * 3);
    memcpy(rbb_corner, rbb_corner_cpy, sizeof(float) * 3);
}

void loop() {
    
}
float getDist(float pt1[3], float pt2[3]) {
    return sqrtf(square(pt2[2] - pt1[2]) + square(pt2[1] - pt1[1]) + square(pt2[0] - pt1[0]));
}

float MAX(float a, float b) {
    if (a >= b) return a;
    return b;
} 

float* midpt(float t_pt[3], float b_pt[3]) {
    /*finds the midpoint of a quadrilateral in R^3 given a diagonal in the quadrilateral.
    it is assumed the quadrilateral is parallel to one of the 3 axes*/
    float x_diff = b_pt[0] - t_pt[0];
    float y_diff = b_pt[1] - t_pt[1];
    float z_diff = b_pt[2] - t_pt[2];
    bool t_top;
    //we have to figure out which one is the "top" point and which is the "bottom".
    if (x_diff == 0) t_top = z_diff < 0;
        /*X in this case is the vertical axis. If the vertical is same between diagonals, then the "height"
        difference is measured in the depth axis -- in our case, z. "deeper" point is top*/
    else t_top = x_diff < 0;
    //depth or horizontal axis is the same. either way, the "height" is the vertical axis.
    
    if (!t_top) {
        float swap[3];
        memcpy(swap, t_pt, sizeof(float) * 3);
        memcpy(t_pt, b_pt, sizeof(float) * 3);
        memcpy(b_pt, swap, sizeof(float) * 3);
        
    }
    float ans[3];
    ans[0] = t_pt[0] + 0.5 * x_diff;
    ans[1] = t_pt[1] + 0.5 * y_diff;
    ans[2] = t_pt[2] + 0.5 * z_diff;
    return ans;
}

/*bool willCollide(float pt[3]) {
    //computes whether the straight line path from the sphere to pt will collide with the asteroid.
    float rad = AST_RAD;
    float dist = getDist(AST_CENTER, pt);
    return dist > rad; //broken
}*/

bool willCollide() {
    float ourState[12];
    api.getMyZRState(ourState);
    float ourPos[2];
    ourPos[0] = ourState[0];
    ourPos[1] = ourState[1];
    ourPos[2] = ourState[2];
    return getDist(ourPos, AST_CENTER) < EPSILON;
}

float shadDist() {
    //finds the distance from the shadow to the player
    float ourState[12];
    api.getMyZRState(ourState);
    float ourPos[2];
    ourPos[0] = ourState[0];
    ourPos[1] = ourState[1];
    ourPos[2] = ourState[2];
    float midpt_left[3];
    float midpt_right[3];
    float midpt_bottom[3];
    float midpt_top[3];
    float* first_pt;
    /*Now we find the distance from the point to the midpoints of all 4 potential faces. 
    (2 of them are unreachable). The lowest dist is our answer*/
    first_pt = midpt(ltf_corner, lbb_corner);
    pt_cpy(midpt_left, *first_pt, *(first_pt + 1), *(first_pt + 2));
    first_pt = midpt(rtf_corner, rbb_corner);
    pt_cpy(midpt_right, *first_pt, *(first_pt + 1), *(first_pt + 2));
    first_pt = midpt(ltb_corner, rtf_corner);
    pt_cpy(midpt_top, *first_pt, *(first_pt + 1), *(first_pt + 2));
    first_pt = midpt(lbf_corner, rbb_corner);
    pt_cpy(midpt_bottom, *first_pt, *(first_pt + 1), *(first_pt + 2));
    float dist1 = MAX(getDist(midpt_left, ourPos), getDist(midpt_right, ourPos));
    float dist2 = MAX(getDist(midpt_top, ourPos), getDist(midpt_bottom, ourPos));
    return MAX(dist1,dist2);
}
void pt_cpy(float pt[3], float x, float y, float z) {
    pt[0] = x;
    pt[1] = y;
    pt[2] = z;
}

float ptLineDist(float end1[3], float end2[3], float pt[3]) {
    /*finds the distance from line segment with endpoints [end1, end2] and point pt
    This used to be used in shadDist() in 2D mode. No longer. Gonna keep it anyways. 
    Maybe someone will use it. */

    float new_seg[3];
    new_seg[0] = end2[0] - end1[0]; 
    new_seg[1] = end2[1] - end1[1];
    new_seg[2] = end2[2] - end1[2];
    float len2 = square(new_seg[0]) + square(new_seg[1]) + square(new_seg[2]); 
    //this is just getDist(end1, end2)^2, but the reasoning is that the extra computation of the sqrt is unnecessary.

    float proj = ((pt[0] - end1[0]) * new_seg[0] + (pt[1] - end1[1]) * new_seg[1] + (pt[2] - end1[2]) * new_seg[2]);
    proj = proj / len2;
    //projection of pt 
    if (proj > 1) 
        proj = 1;
    else if (proj < 0)
        proj = 0;
    float pt_on_line[2];
    pt_on_line[0] = end1[0] + proj * new_seg[0];
    pt_on_line[1] = end1[1] + proj * new_seg[1];
    return getDist(pt_on_line,pt);
    
}

float ptCircleDist(float center[3], float rad, float pt[3]) {
    /*returns the distance between point and circle. might be useful for calculating distance to 
    outer zone or danger zone? who knows. also we have to use some Top Gun reference in one of our 
    DEBUG(())s relating to "the danger zone". like if we get too close to the danger zone then print
    "Ridin' to the Danger Zone!" or something. */
    float ans = getDist(center,pt) - rad;
    return fabsf(ans);                            
}
float square(float x) {
    return x*x;
}
