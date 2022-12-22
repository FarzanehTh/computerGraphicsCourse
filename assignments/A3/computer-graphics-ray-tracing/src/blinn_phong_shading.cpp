#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
    Eigen::Vector3d colour(0.0, 0.0, 0.0);
    // Compute and add a global Ambient shading
    double ambientLight = 0.1;
    colour += objects[hit_id]->material->ka * ambientLight;

    double epsilon = 0.000001;

    for (int i = 0; i < lights.size(); i++) {
        Eigen::Vector3d L;
        double max_t;
        Eigen::Vector3d intersectionPoint = ray.origin + t * ray.direction;
        // Get the vector of light ray L from a directional or point light source
        lights[i]->direction(intersectionPoint, L, max_t);

        // Create a shadow ray
        // Add an epsioln to t so that we can consider the ray from [epsioln, inf] instead of [0, inf].
        // This makes sure we do not consider the ray hitting the surface where the intersectionPoint is on
        // by calculation errors.
        Ray shadowRay;
        shadowRay.origin = intersectionPoint + epsilon * n;
        shadowRay.direction = L;
        // Use the colour of the object only if this point is not in the shadow of another object
        // min_t for this call is the max_t that we found. That makes sure that we only consider
        // a shadow from an object if it is within the range of the intersection point and 
        // the source of light and not beyond this source of light.
        double tt;
        int hit_idd;
        Eigen::Vector3d nn;
        
        // Shadow ray is in fact for us to check if this light ray that we have will
        // hit the object or it will be blocked by another object in the scene.
        // So colour the point if it either, the shadow ray not hitting any objects to block the light
        // to it, OR if the light source is a directional light like the sun,
        // OR if the hit point to an object is beyond the distance of this point and the
        // light source. In either of these cases, the light is hitting the intersection point
        // and so we can normally calculate the effect of the diffuse and specular lights on its colour.
        if (!first_hit(shadowRay, 0, objects, hit_idd, tt, nn) ||
            max_t == std::numeric_limits<double>::infinity() ||
            (max_t != std::numeric_limits<double>::infinity() && tt > max_t) ) {
            // Compute Lambertian shading
            // We will let the radius for the light fall off to be equal to 1.
            Eigen::Vector3d adjustedDiffuseIntensity = (objects[hit_id]->material->kd.array() * (lights[i]->I / 1.0).array()).matrix();
            double nDotL = std::max((double)0, n.dot(L));
            colour += adjustedDiffuseIntensity * nDotL;

            // Compute Specular shading
            Eigen::Vector3d adjustedSpecularIntensity = (objects[hit_id]->material->ks.array() * lights[i]->I.array()).matrix();
            // The ray hitting the intersection point is in fact the "direction" component of
            // the hitting ray "ray", which we have to reverse it to compute the h.
            // Since here we only care about the direction of the vector hitting p
            // not its origin position, so -ray.direction.normalized() is enough.
            Eigen::Vector3d h = (-ray.direction.normalized() + L).normalized();
            double nDotH = std::max((double)0, n.dot(h));
            colour += adjustedSpecularIntensity * pow(nDotH, objects[hit_id]->material->phong_exponent);
        }
    }
    // Make sure the rgb values remain between 0-1
    colour << std::min(std::max(colour[0], 0.0), 1.0), std::min(std::max(colour[1], 0.0), 1.0), std::min(std::max(colour[2], 0.0), 1.0);
    return colour;
}
