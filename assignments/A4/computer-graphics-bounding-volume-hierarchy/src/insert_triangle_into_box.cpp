#include "insert_triangle_into_box.h"
#include <algorithm> 

void insert_triangle_into_box(
  const Eigen::RowVector3d & a,
  const Eigen::RowVector3d & b,
  const Eigen::RowVector3d & c,
  BoundingBox & B)
{
    // Compute new min_corner of B
    for (int i = 0; i < 3; i++) {
        B.min_corner(i) = std::min({B.min_corner(i), a(i), b(i), c(i)});
    }

    // Compute new max_corner of B
    for (int i = 0; i < 3; i++) {
        B.max_corner(i) = std::max({B.max_corner(i), a(i), b(i), c(i)});
    }
}


