#include "box_box_intersect.h"
bool box_box_intersect(
  const BoundingBox & A,
  const BoundingBox & B)
{
    Eigen::RowVector3d centerA = 0.5 * (A.max_corner + A.min_corner);
    Eigen::RowVector3d v1(A.min_corner(0), centerA(1), centerA(2));
    double halfEdgeSizeA = std::abs((centerA - v1).norm());

    Eigen::RowVector3d centerB = 0.5 * (B.max_corner + B.min_corner);
    Eigen::RowVector3d v2(B.min_corner(0), centerB(1), centerB(2));
    double halfEdgeSizeB = std::abs((centerB - v2).norm());

    Eigen::RowVector3d centersDiff;
    centersDiff << std::abs(centerB(0) - centerA(0)), std::abs(centerB(1) - centerA(1)), std::abs(centerB(2) - centerA(2));

    if (centersDiff(0) != 0 && centersDiff(0) - halfEdgeSizeA - halfEdgeSizeB <= 0) {
        // Boxes intersect
        return true;
    }
    if (centersDiff(1) != 0 && centersDiff(1) - halfEdgeSizeA - halfEdgeSizeB <= 0) {
        // Boxes intersect
        return true;
    }
    if (centersDiff(2) != 0 && centersDiff(2) - halfEdgeSizeA - halfEdgeSizeB <= 0) {
        // Boxes intersect
        return true;
    }
    return false;
}

