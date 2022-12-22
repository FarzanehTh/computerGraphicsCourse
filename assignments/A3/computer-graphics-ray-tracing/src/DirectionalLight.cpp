#include "DirectionalLight.h"
#include <limits>

void DirectionalLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{
    // The directional light is just the normalized reversed direction of the light
    // which is the same for all the points in the scene. 
    // We reverse it since we want to calculate the light from the point q towards the light source.
    d = (- this->d).normalized();
    max_t = std::numeric_limits<double>::infinity();
}

