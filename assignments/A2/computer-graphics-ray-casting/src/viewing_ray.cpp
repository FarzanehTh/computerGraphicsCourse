#include "viewing_ray.h"
#include <iostream>

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
    ray.origin = camera.e;

    // Compute the values of u(i) and v(j) which are the mapped values from
    // the (i, j) pixel into the camera's space
    double u = (camera.width / width) * (j + 0.5) - (camera.width / 2.0);
    double v = (camera.height / height) * (i + 0.5) - (camera.height / 2.0);

    Eigen::Vector3d vector; // It is a (3 * 1) vector
    // Since we consider the origin to be the top left pixel (rather than bottom left in the book),
    // we have to use -v since the origin of the image starts from the top left corner
    // with positive values pointing down, while in the camera space basis, positive v points up.
    vector << u, -v, -camera.d;
    Eigen::Matrix3d T;
    T.col(0) << camera.u;
    T.col(1) << camera.v;
    T.col(2) << camera.w;
    Eigen::Vector3d transformedVector;
    transformedVector << T * vector;
    ray.direction = transformedVector;
}

