float angleBetweenAtt(float A[3], float B[3]) {
    
    // The dot product can be calculated from the components of two vectors.
    // It can also be calculated from their magnitudes and the angle between them.
    // Setting these two equal, we can solve for the angle. 
    // This gives us a generalized equation for the angle between two
    // vectors in 3-space.
    
    // This version is better suited for attitudes to minimize 
    // wasted computations.
    
    float dotProduct = mathVecInner(A, B, 3);
    
    float angle = acos(dotProduct);
    
    return angle; 
}