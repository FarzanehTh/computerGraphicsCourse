#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Geometry>

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
    Eigen::Vector3d a = std::get<0>(this->corners);
    Eigen::Vector3d b = std::get<1>(this->corners);
    Eigen::Vector3d c = std::get<2>(this->corners);

    n = (a - b).cross(a - c).normalized();

    // Solution #1: Solving the system of equations using the "Cramer's rule"
    //  https://en.wikipedia.org/wiki/Cramer's_rule
    /*
    Eigen::Vector3d resultVector;
    resultVector << a - ray.origin;

    Eigen::Matrix3d A(3, 3);
    A.row(0) << a[0] - b[0], a[0] - c[0], ray.direction[0];
    A.row(1) << a[1] - b[1], a[1] - c[1], ray.direction[1];
    A.row(2) << a[2] - b[2], a[2] - c[2], ray.direction[2];

    Eigen::Matrix3d B1(3, 3);
    B1 << A;
    B1.col(2) << resultVector;

    Eigen::Matrix3d B2(3, 3);
    B2 << A;
    B2.col(1) << resultVector;

    Eigen::Matrix3d B3(3, 3);
    B3 << A;
    B3.col(0) << resultVector;

    n = (a - c).cross(a - b).normalized();
    t = ((double)B1.determinant()) / A.determinant();

    if (t < min_t) {
        return false;
    }

    double gamma = ((double)B2.determinant()) / A.determinant();

    if (gamma < 0 || gamma > 1) {
        return false;
    }

    double beta = ((double)B3.determinant()) / A.determinant();

    if (beta < 0 || beta > 1 - gamma) {
        return false;
    }

    return true;
    */

    // Solution #2: Solving the system of equations using the Eigen's colPivHouseholderQr
    // We can use other types of solving them also, from here:
    // https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html

    Eigen::Matrix3d A;
    A << a - b, a - c, ray.direction;

    Eigen::Vector3d result;
    result << a - ray.origin;

    Eigen::Vector3d x = A.colPivHouseholderQr().solve(result);

    // x is a 3 * 1 vector containing (beta, gamma, t)
    double beta = x[0];
    double gamma = x[1];
    t = x[2];

    if (t < min_t) {
        return false;
    }
    if (beta < 0 || gamma < 0 || beta + gamma >= 1) {
        return false;
    }

    return true;
}