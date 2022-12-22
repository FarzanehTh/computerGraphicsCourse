// Inputs:
//   t  3D vector by which to translate
// Return a 4x4 matrix that translates and 3D point by the given 3D vector
mat4 translate(vec3 t)
{
  vec4 translationVector = vec4(t.x, t.y, t.z, 1.0);
  mat4 matrix = identity();
  // Set the last column of it to be the translationVector
  matrix[3] = translationVector; 
  return matrix;
}

