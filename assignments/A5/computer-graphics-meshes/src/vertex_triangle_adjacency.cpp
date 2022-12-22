#include "vertex_triangle_adjacency.h"

void vertex_triangle_adjacency(
  const Eigen::MatrixXi & F,
  const int num_vertices,
  std::vector<std::vector<int> > & VF)
{
    VF.resize(num_vertices);
    // For each vertex store a list of all incident faces.
    for (int f = 0; f < F.rows(); f++) {
        // F.cols() for triangle faces is equal to 3.
        for (int i = 0; i < F.cols(); i++) {
            VF[F.row(f)(i)].push_back(f);
        }
    }
}

