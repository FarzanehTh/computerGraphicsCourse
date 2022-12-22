#include "PointLight.h"
#include <cmath>

void PointLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{
    // The positional light, the direction of light is the normalized
    // result of (light's point - intersection point)
    d = (this->p - q).normalized();
    max_t = sqrt(pow(d[0], 2.0) + pow(d[1], 2.0) + pow(d[2], 2.0));
}
