#include "ray_intersect_box.h"
#include <iostream>
#include <algorithm> 

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  const double max_t)
{

    double boxXmin = box.min_corner(0);
    double boxYmin = box.min_corner(1);
    double boxZmin = box.min_corner(2);

    double boxXmax = box.max_corner(0);
    double boxYmax = box.max_corner(1);
    double boxZmax = box.max_corner(2);

    double directionVectorX = 1 / ray.direction(0);
    double directionVectorY = 1 / ray.direction(1);
    double directionVectorZ = 1 / ray.direction(2);

    double tXMin;
    double tYMin;
    double tZMin;

    double tXMax;
    double tYMax;
    double tZMax;

    if (directionVectorX >= 0) {
        tXMin = directionVectorX * (boxXmin - ray.origin(0));
        tXMax = directionVectorX * (boxXmax - ray.origin(0));
    } else {
        tXMin = directionVectorX * (boxXmax - ray.origin(0));
        tXMax = directionVectorX * (boxXmin - ray.origin(0));
    }

    if (directionVectorY >= 0) {
        tYMin = directionVectorY * (boxYmin - ray.origin(1));
        tYMax = directionVectorY * (boxYmax - ray.origin(1));
    } else {
        tYMin = directionVectorY * (boxYmax - ray.origin(1));
        tYMax = directionVectorY * (boxYmin - ray.origin(1));
    }

    if (directionVectorZ >= 0) {
        tZMin = directionVectorZ * (boxZmin - ray.origin(2));
        tZMax = directionVectorZ * (boxZmax - ray.origin(2));
    } else {
        tZMin = directionVectorZ * (boxZmax - ray.origin(2));
        tZMax = directionVectorZ * (boxZmin - ray.origin(2));
    }

    if (tXMin < min_t || tYMin < min_t || tZMin < min_t ||
        tXMax < min_t || tYMax < min_t || tZMax < min_t) {
            return false;
    }

    if (tXMin > max_t || tYMin > max_t || tZMin > max_t ||
        tXMax > max_t || tYMax > max_t || tZMax > max_t) {
            return false;
    }

    if (std::max({tXMin, tYMin, tZMin}) < std::min({tXMax, tYMax, tZMax})) {
        // The ray hits the box.
        return true;
    }

    return false;
}
