#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
    double denominator = this->normal.dot(ray.direction);
    if (denominator == 0) {
        return false;
    }
    double tRequired = ((double)((-1.0 * this->normal).dot(ray.origin - this->point))) / denominator;
    // t should be greater than or equal to the min_t
    if (tRequired < min_t) {
        return false;
    }
    t = tRequired;
    n = this->normal.normalized();
    return true;
}

