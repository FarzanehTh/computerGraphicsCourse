// Inputs:
//   s  amount to scale in all directions
// Return a 4x4 matrix that scales and input 3D position/vector by s in all 3
// directions.
mat4 uniform_scale(float s)
{
  mat4 scaledMatrix = identity() * s;
  // Change the last column
  scaledMatrix[3] = vec4(0, 0, 0, 1);
  return scaledMatrix;
}

