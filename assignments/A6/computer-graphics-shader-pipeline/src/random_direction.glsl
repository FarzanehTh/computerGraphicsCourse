// Generate a pseudorandom unit 3D vector
// 
// Inputs:
//   seed  3D seed
// Returns psuedorandom, unit 3D vector drawn from uniform distribution over
// the unit sphere (assuming random2 is uniform over [0,1]²).
//
// expects: random2.glsl, PI.glsl
vec3 random_direction( vec3 seed)
{
    vec2 random2DVector = random2(seed);
    // Adjust the values of random2DVector = (x, y) to be between 0-2M_PI and 0-M_PI.
    // This way x will be the phi and y will be the theta that represent a point on a unit sphere.
    random2DVector = vec2(random2DVector.x * 2 * M_PI, random2DVector.y * M_PI);
    float phi = random2DVector.x;
    float theta = random2DVector.y;
    vec3 random3DResult = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    return normalize(random3DResult);
}
