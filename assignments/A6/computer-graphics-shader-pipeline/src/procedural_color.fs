// Set the pixel color to an interesting procedural color generated by mixing
// and filtering Perlin noise of different frequencies.
//
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
out vec3 color;

// expects: blinn_phong, perlin_noise
void main()
{
    // I used similar approach mentioned in slide 44 here:
    // https://resources.mpi-inf.mpg.de/departments/d4/teaching/ws200708/cg/slides/CG08-Procedural.pdf
    float moonNoise = sin(100 * (view_pos_fs_in.x + view_pos_fs_in.y) + perlin_noise(view_pos_fs_in.xyz * 100));
    float planetNoise = 2 * sin(view_pos_fs_in.x + perlin_noise(view_pos_fs_in.xyz * 10));

    // Apply a rotating directional light
    float theta = ((2.0 * M_PI) / 4.0) * animation_seconds;
    vec3 lightVector = vec3(6, 10, 3000);
    mat4 rotationMatrix = mat4(
        cos(theta), 0, -sin(theta), 0,
        0, 1.0, 0, 0,
        sin(theta), 0, cos(theta), 0,
        0, 0, 0, 1.0);
    vec4 circularDirectionalLight = rotationMatrix * vec4(lightVector, 1);
    // Project the circularDirectionalLight to the view space
    vec3 viewDirectionalLight = (view * circularDirectionalLight).xyz;
    vec3 ka = vec3(0.02, 0.02, 0.02);
    vec3 kd = is_moon ? moonNoise * vec3(0.5, 0.5, 0.5) : planetNoise * vec3(0.01, 0.2, 0.8);
    vec3 ks = vec3(0.8, 0.7, 0.6);
    // Since eye/camera is at (0, 0, 0) so (eye - view_pos_fs_in) is -view_pos_fs_in.
    vec3 v = -normalize(view_pos_fs_in.xyz);
    vec3 l = normalize(viewDirectionalLight);
    color = blinn_phong(ka, kd, ks, 1000.0, normalize(normal_fs_in), v, l);
}
