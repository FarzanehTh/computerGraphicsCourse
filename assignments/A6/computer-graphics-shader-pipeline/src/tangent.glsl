// Input:
//   N  3D unit normal vector
// Outputs:
//   T  3D unit tangent vector
//   B  3D unit bitangent vector
void tangent(in vec3 N, out vec3 T, out vec3 B)
{
    // We know the spherical cooridinates of a point p on the sphere
    // is: vec3 p = vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
    float phi = atan(N.y, N.x);
    float theta = acos(N.z);
    if (phi < 0) {
        phi = 2 * M_PI + phi;
    }

    // So the partial derivative of this equation with respect to theta,
    // at the (phi, theta) will be a tangent vector T.
    T = vec3(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta));
    T = normalize(T);

    // The bi-tangent at the point p is the cross product of N and T
    B = normalize(cross(N, T));
}
