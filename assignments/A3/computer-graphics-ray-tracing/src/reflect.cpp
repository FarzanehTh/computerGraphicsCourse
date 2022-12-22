#include <Eigen/Core>

Eigen::Vector3d reflect(const Eigen::Vector3d & in, const Eigen::Vector3d & n)
{
    // in is in fact the -L, since we want the ray to be towards out of the intersection point.
    // So in the formula of reflection = 2*(n.L)n-L, we already have -L which is in.
    return (2 * n.dot(-in) * n + in).normalized();
}
