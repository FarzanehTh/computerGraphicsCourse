#include "first_hit.h"

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
    // Go over the objects and find the closest hit.
    // i.e. the one with smallest t.
    double tMinSoFar = std::numeric_limits<double>::infinity();
    Eigen::Vector3d nTemp;
    double tTemp = 0;
    bool found = false;

    for (int i = 0; i < objects.size(); i++) {
        // min_t makes sure that we calculate the intersection of the ray with the objects 
        // in fornt of the camera and not the ones in the back of it
        if (objects[i]->intersect(ray, min_t, tTemp, nTemp)) {
            if (tTemp < tMinSoFar) {
                tMinSoFar = tTemp;
                t = tTemp;
                n = nTemp;
                hit_id = i;
                found = true;
            }
        }
    }
    return found;
}
