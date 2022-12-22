// Set the pixel color using Blinn-Phong shading (e.g., with constant blue and
// gray material color) with a bumpy texture.
// 
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
//                     linearly interpolated from tessellation evaluation shader
//                     output
in vec3 sphere_fs_in;
in vec3 normal_fs_in; // view and model transformed 3D normal
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
//               rgb color of this pixel
out vec3 color;
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
    // 1. Compute the bump on the p in the "local" space 
    vec3 p = sphere_fs_in;
    vec3 pWithBump = bump_position(is_moon, p);

    // 2. Compute the tangent and bi-tangent vectors. Note that the normal vector
    // in the "local" space is equal to the sphere_fs_in itself.
    vec3 T;
    vec3 B;
    tangent(sphere_fs_in, T, B);

    // 3. Compute the bump normal vector on the pWithBump point in the "local" space.
    float epsilon = 0.0001;
    vec3 derivativeWithRespectToT = (bump_position(is_moon, p + epsilon * T) - pWithBump) / epsilon;
    vec3 derivativeWithRespectToB = (bump_position(is_moon, p + epsilon * B) - pWithBump) / epsilon;
    vec3 bumpNormal = normalize(cross(derivativeWithRespectToT, derivativeWithRespectToB));

    // 4. Compute the bump normal vector on the pWithBump point in the "camera" space.
    // Based on the proof in the book, the bumpNormal will need to be transformed by
    // the inverse of transpose of the original matrices.
    mat4 model = model(is_moon, animation_seconds);
    bumpNormal = (inverse(transpose(view)) * inverse(transpose(model)) * vec4(bumpNormal, 1.0)).xyz;

    // 5. Apply a rotating directional light based on the bumpNormal in the "camera" space.
    float theta = ((2.0 * M_PI) / 4.0) * animation_seconds;
    vec3 lightVector = vec3(6, 10, -300);
    mat4 rotationMatrix = mat4(
        cos(theta), 0, -sin(theta), 0,
        0, 1.0, 0, 0,
        sin(theta), 0, cos(theta), 0,
        0, 0, 0, 1.0);
    vec4 circularDirectionalLight = rotationMatrix * vec4(lightVector, 1);
    vec3 viewDirectionalLight = (view * circularDirectionalLight).xyz;
    vec3 ka = vec3(0.02, 0.02, 0.02);
    vec3 kd = is_moon ? vec3(0.5, 0.5, 0.5) : vec3(0.01, 0.2, 0.8);
    vec3 ks = vec3(0.8, 0.7, 0.6);
    // Since eye/camera is at (0, 0, 0) so (eye - view_pos_fs_in) is -view_pos_fs_in.
    vec3 v = -normalize(view_pos_fs_in.xyz);
    vec3 l = normalize(viewDirectionalLight);
    color = blinn_phong(ka, kd, ks, 1000, normalize(bumpNormal), v, l);
}
