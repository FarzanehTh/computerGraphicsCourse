#include "point_box_squared_distance.h"
#include <vector>
#include <Eigen/Geometry>
#include <iostream>

double point_box_squared_distance(
  const Eigen::RowVector3d & query,
  const BoundingBox & box)
{
    // We need to find the projection of the point query on the box (which is a cube)
    // and then find the distance of that projection point to the query point.
    // The projection point p will be the straight line from the query point to it.

    // To find p, we need to adjust the dimensions of the query point to be within the box.
    Eigen::RowVector3d p;
    p << query;
    for (int i = 0; i < 3; i++) {
        if (query(i) < box.min_corner(i)) {
            p(i) = box.min_corner(i);
        } else if (query(i) > box.max_corner(i)) {
            p(i) = box.max_corner(i);
        }
        // else the query value in the i-th dimension is within the box already
    }

    // Now the distance between the query and point p is the distance between
    // the query point and the box.
    return pow((query - p).norm(), 2.0);
}
