#include "ray_intersect_triangle.h"
#include <Eigen/Geometry>

bool ray_intersect_triangle(
  const Ray & ray,
  const Eigen::RowVector3d & A,
  const Eigen::RowVector3d & B,
  const Eigen::RowVector3d & C,
  const double min_t,
  const double max_t,
  double & t)
{
    // Solving the system of equations using the Eigen's colPivHouseholderQr
    // We can use other types of solving them also, from here:
    // https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html

    Eigen::Matrix3d M;
    M << (A - B).transpose(), (A - C).transpose(), ray.direction;

    Eigen::Vector3d result;
    result << A.transpose() - ray.origin;

    Eigen::Vector3d x = M.colPivHouseholderQr().solve(result);

    // x is a 3 * 1 vector containing (beta, gamma, t)
    double beta = x[0];
    double gamma = x[1];
    t = x[2];

    if (t < min_t || t > max_t) {
        return false;
    }
    if (beta < 0 || gamma < 0 || beta + gamma >= 1) {
        return false;
    }

    return true;
}

