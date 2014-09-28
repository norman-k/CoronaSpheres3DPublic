// Added by Elvin

float angleBetween(float A[3], float B[3]) {
    
    // The dot product can be calculated from the components of two vectors.
    // It can also be calculated from their magnitudes and the angle between them.
    // Setting these two equal, we can solve for the angle. 
    // This gives us a generalized equation for the angle between two
    // vectors in 3-space.
    
    // Note that you don't need to use this function when you're
    // working with attitudes. Attitudes are normalized vectors 
    // and there will be some wasted operations here. 
    
    float dotProduct = mathVecInner(A, B, 3);
    float magnitudeA = mathVecMagnitude(A, 3);
    float magnitudeB = mathVecMagnitude(B, 3);
    
    float angle = acos(dotProduct / (magnitudeA * magnitudeB));
    
    return angle; 
}