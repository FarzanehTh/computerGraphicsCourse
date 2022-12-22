// Create a bumpy surface by using procedural noise to generate a height (
// displacement in normal direction).
//
// Inputs:
//   is_moon  whether we're looking at the moon or centre planet
//   s  3D position of seed for noise generation
// Returns elevation adjust along normal (values between -0.1 and 0.1 are
//   reasonable.
float bump_height( bool is_moon, vec3 s)
{
    // vec3 noiseRandomVector = is_moon ? 2.5 * s : 10.0 * sin(s);
    vec3 noiseRandomVector = is_moon ? 2.5 * s : 10.0 * sin(s);
    float smoothNoise = improved_perlin_noise(noiseRandomVector);
    // Clamp the smoothNoise to be between -0.1 and 0.1
    return clamp(smoothNoise, -0.1, 0.1);
}
/*
 // vec3 noiseRandomVector = is_moon ? 2.5 * s : 10.0 * sin(s);
    float smoothNoise = improved_perlin_noise(noiseRandomVector);
    vec3 noiseRandomVector = is_moon ? smooth_heaviside(smoothNoise, 2) : smooth_heaviside(smoothNoise, 10);
    // Clamp the smoothNoise to be between -0.1 and 0.1
    return clamp(noiseRandomVector, -0.1, 0.1);
    */