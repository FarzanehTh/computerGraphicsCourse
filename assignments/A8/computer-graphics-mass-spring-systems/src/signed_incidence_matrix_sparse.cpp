#include "signed_incidence_matrix_sparse.h"
#include <vector>

void signed_incidence_matrix_sparse(
  const int n,
  const Eigen::MatrixXi & E,
  Eigen::SparseMatrix<double>  & A)
{
    // n number of vertices (#V)
    // E #E by 2 list of edge indices into rows of V
    std::vector<Eigen::Triplet<double>> ijv;
    for (int i = 0; i < E.rows(); i++) {
        // A triplet consists of (row, col, value)
        ijv.emplace_back(i, E(i, 0), 1);
        ijv.emplace_back(i, E(i, 1), -1);
    }
    A.resize(E.rows(), n);
    A.setFromTriplets(ijv.begin(), ijv.end());
}
