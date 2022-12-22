#include "insert_box_into_box.h"
#include <algorithm>

void insert_box_into_box(
  const BoundingBox & A,
  BoundingBox & B)
{
    // To put the box A into the box B, we should compute the new min_corner and
    // max_corner for the box B.

    // Compute new min_corner for B
    for (int i = 0; i < 3; i++) {
        B.min_corner(i) = std::min(B.min_corner(i), A.min_corner(i));
    }

    // Compute new max_corner for B
    for (int i = 0; i < 3; i++) {
        B.max_corner(i) = std::max(B.max_corner(i), A.max_corner(i));
    }
}

