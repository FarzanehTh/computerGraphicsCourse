// Filter an input value to perform a smooth step. This function should be a
// cubic polynomial with smooth_step(0) = 0, smooth_step(1) = 1, and zero first
// derivatives at f=0 and f=1. 
//
// Inputs:
//   f  input value
// Returns filtered output value
float smooth_step( float f)
{
    // Using the Smoothstep function mentioned here: https://en.wikipedia.org/wiki/Smoothstep
    if (f <= 0) {
        return 0;
    } 
    if (f >= 1.0) {
        return 1.0;
    }
    return 3.0 * pow(f, 2) - 2.0 * pow(f, 3);
}

vec3 smooth_step( vec3 f)
{
  return vec3(smooth_step(f.x), smooth_step(f.y), smooth_step(f.z));
}
