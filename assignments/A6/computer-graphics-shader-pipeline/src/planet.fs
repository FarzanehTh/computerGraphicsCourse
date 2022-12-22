// Generate a procedural planet and orbiting moon. Use layers of (improved)
// Perlin noise to generate planetary features such as vegetation, gaseous
// clouds, mountains, valleys, ice caps, rivers, oceans. Don't forget about the
// moon. Use `animation_seconds` in your noise input to create (periodic)
// temporal effects.
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
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent

vec3 getBumpNormal(vec3 p) {
    vec3 pWithBump = bump_position(is_moon, p);

    // 1. Compute the tangent and bi-tangent vectors. Note that the normal vector
    // in the "local" space is the sphere_fs_in itself.
    vec3 T;
    vec3 B;
    tangent(sphere_fs_in, T, B);

    // 2. Compute the bump normal vector on the pWithBump point in the "local" space.
    float epsilon = 0.0001;
    vec3 derivativeWithRespectToT = (bump_position(is_moon, p + epsilon * T) - pWithBump) / epsilon;
    vec3 derivativeWithRespectToB = (bump_position(is_moon, p + epsilon * B) - pWithBump) / epsilon;
    vec3 bumpNormal = normalize(cross(derivativeWithRespectToT, derivativeWithRespectToB));

    // 3. Compute bump normal
    mat4 model = model(is_moon, animation_seconds);
    bumpNormal = (inverse(transpose(view)) * inverse(transpose(model)) * vec4(bumpNormal, 1.0)).xyz;
    return bumpNormal;
}

vec3 applyColor(vec3 p, vec3 normal, vec3 ka, vec3 kd, vec3 ks) {
    float theta = ((2.0 * M_PI) / 4.0) * animation_seconds;
    vec3 lightVector = vec3(6, 10, -300);
    mat4 rotationMatrix = mat4(
        cos(theta), 0, -sin(theta), 0,
        0, 1.0, 0, 0,
        sin(theta), 0, cos(theta), 0,
        0, 0, 0, 1.0);
    vec4 circularDirectionalLight = rotationMatrix * vec4(lightVector, 1);
    vec3 viewDirectionalLight = (view * circularDirectionalLight).xyz;
    color = blinn_phong(ka, kd, ks, 1000.0, normal, p, normalize(viewDirectionalLight));
    return color;
}

vec3 createTurbulance(vec3 p) {
    float value1 = sin(0.5 * p.x) + cos(p.x * p.x) + tan(0.8 * p.x);
    float value2 = sin(p.x) + cos(p.x * p.x) + tan(p.x);
    float value3 = sin(0.2 * p.x) + cos(p.x * p.x) + tan(10 * p.x);
    return vec3(clamp(value1, -0.1, 0.1), clamp(value2, 0, 1), clamp(value3, 0.1, 0.5) * cos(10 * p.z));
}

void main()
{
    vec3 normal = normal_fs_in;
    vec3 ka, kd, ks;
    ka = vec3(0.02, 0.02, 0.02);
    kd = is_moon ? vec3(0.5, 0.5, 0.5) : vec3(0.01, 0.2, 0.8);
    ks = vec3(0.8, 0.7, 0.6);

    // 1. Compute the bump height
    float theta = ((2.0 * M_PI) / 4.0) * animation_seconds;
    vec3 p = sphere_fs_in;
    float bumpHeight = bump_height(is_moon, view_pos_fs_in.xyz);

    // 2. Create bumps only in certain random times or bump heights,
    // and specify the diffuse/specular values randomly to create textures.
    if (animation_seconds + 3 > 0.5 * sphere_fs_in.x) {
        normal = getBumpNormal(p);
        ka = is_moon ? vec3(0.03, 0.04, 0.03) : vec3(0.01, 0.02, 0.3);
        kd = is_moon ? vec3(0.5, 0.5, 0.5) :
            vec3(0.5 * cos((view_pos_fs_in.x + view_pos_fs_in.y + view_pos_fs_in.z) * theta), 3.5 * sin(1 * view_pos_fs_in.z * theta), 0.7);
        ks = is_moon ? vec3(0.8, 0.7, 0.8): vec3(0.1 * tan(view_pos_fs_in.x), 10 * sin(view_pos_fs_in.z), 0.1);
    }
    if (bumpHeight > 0.025) {
        normal = getBumpNormal(p);
        ka = is_moon ? vec3(0.03, 0.04, 0.03) : vec3(0.01, 0.02, 0.3);
        kd = is_moon ? vec3(0.5, 0.5, 0.5) :
            vec3(0.5 * cos((view_pos_fs_in.x + view_pos_fs_in.y + view_pos_fs_in.z) * theta), 3.5 * sin(1 * view_pos_fs_in.z * theta), 0.7);
        ks = vec3(0.1 * tan(view_pos_fs_in.x), 10 * sin(view_pos_fs_in.z), 0.1);
    }
    color = applyColor(-normalize(view_pos_fs_in.xyz), normalize(normal), ka, kd, ks);
}
