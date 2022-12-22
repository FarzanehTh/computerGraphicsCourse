#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
    int hit_id;
    double t;
    Eigen::Vector3d n;

    if (first_hit(ray, min_t, objects, hit_id, t, n)) {
        // Compute local shading
        rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);

        // Compute global shading
        double epsilon = 0.000001;
        Eigen::Vector3d intersectionPoint = ray.origin + t * ray.direction;
        // The ray.direction is in fact the vector that hits the intersection point
        Eigen::Vector3d reflectedVector = reflect(ray.direction, n);
        Ray reflectedRay;
        // Compute the reflected ray to start epsilon * n after the original ray intersection
        reflectedRay.origin = intersectionPoint + epsilon * n;
        reflectedRay.direction = reflectedVector;
        if (num_recursive_calls < 5) {
            Eigen::Vector3d mirrorColour;
            if(raycolor(reflectedRay, 0, objects, lights, num_recursive_calls + 1, mirrorColour)) {
                rgb += (objects[hit_id]->material->km.array() * mirrorColour.array()).matrix();
            }
        }
        return true;
    } 

    return false;
}
