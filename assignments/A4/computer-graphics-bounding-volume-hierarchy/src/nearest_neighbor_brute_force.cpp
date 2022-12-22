#include "nearest_neighbor_brute_force.h"
#include <limits>// std::numeric_limits<double>::infinity();

void nearest_neighbor_brute_force(
  const Eigen::MatrixXd & points,
  const Eigen::RowVector3d & query,
  int & I,
  double & sqrD)
{
    I = -1;
    sqrD = std::numeric_limits<double>::infinity();

    for (int i = 0; i < points.rows(); i++) {
        Eigen::RowVector3d curPoint = points.row(i);
        double dist =
            pow(query(0) - curPoint(0), 2.0) +
            pow(query(1) - curPoint(1), 2.0) + 
            pow(query(2) - curPoint(2), 2.0);
        if (dist < sqrD) {
            sqrD = dist;
            I = i;
        }
    }
}
