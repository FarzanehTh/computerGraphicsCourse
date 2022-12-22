#include "Sphere.h"
#include "Ray.h"
#include <cmath>

bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
    // Calculate the discriminant of the quadratic equation of the
    // sphere touching the point e+td of the ray.

    double A = ray.direction.dot(ray.direction);
    double B = (2 * ray.direction).dot(ray.origin - this->center);
    double C = (ray.origin - this->center).dot(ray.origin - this->center) - (this->radius * this->radius);

    double discriminant = (B * B) - (4 * A * C);
    if (discriminant < 0) {
        return false;
    }

    double t1 = (-B + sqrt(discriminant)) / (2 * A);
    double t2 = (-B - sqrt(discriminant)) / (2 * A);

    // t2 should be smaller, so start with that
    if (t2 >= min_t) {
        t = t2;
    } else if (t1 >= min_t) {
        t = t1;
    } else {
        return false;
    }
    n = (((ray.origin + t * ray.direction) - this->center) / this->radius).normalized();
    return true;
}

