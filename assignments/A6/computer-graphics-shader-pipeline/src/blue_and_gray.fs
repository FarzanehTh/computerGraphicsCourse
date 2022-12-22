// Set the pixel color to blue or gray depending on is_moon.
//
// Uniforms:
uniform bool is_moon;
// Outputs:
out vec3 color;
void main()
{
  color = is_moon ? vec3(0.5, 0.5, 0.5) : vec3(0 ,0 ,1);
}
