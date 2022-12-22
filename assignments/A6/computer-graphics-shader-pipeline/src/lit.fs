// Add (hard code) an orbiting (point or directional) light to the scene. Light
// the scene using the Blinn-Phong Lighting Model.
//
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
in vec3 sphere_fs_in; // 3D position _before_ applying model, view or projection transformations (e.g., point on unit sphere)
in vec3 normal_fs_in; // view and model transformed 3D normal
in vec4 pos_fs_in; // projected, view, and model transformed 3D position
in vec4 view_pos_fs_in; // view and model transformed 3D position
// Outputs:
out vec3 color;
// expects: PI, blinn_phong
void main()
{
    // Apply a rotating directional light
    float theta = ((2.0 * M_PI) / 4.0) * animation_seconds;
    vec3 lightVector = vec3(6, 100, -300);
    mat4 rotationMatrix = mat4(
        cos(theta), 0, -sin(theta), 0,
        0, 1.0, 0, 0,
        sin(theta), 0, cos(theta), 0,
        0, 0, 0, 1.0);
    vec4 rotatingDirectionalLight = rotationMatrix * vec4(lightVector, 1);
    vec3 viewDirectionalLight = (view * rotatingDirectionalLight).xyz;

    vec3 ka = vec3(0.02, 0.02, 0.02);
    vec3 kd = is_moon ? vec3(0.5, 0.5, 0.55) : vec3(0.01, 0.2, 0.8);
    vec3 ks = vec3(0.8, 0.7, 0.6);
    float p = 1000;
    // Since eye/camera is at (0, 0, 0) so (eye - view_pos_fs_in) is -view_pos_fs_in.
    vec3 v = -normalize(view_pos_fs_in.xyz);
    vec3 l = normalize(viewDirectionalLight);
    color = blinn_phong(ka, kd, ks, p, normalize(normal_fs_in), v, l);
}
