#include "signed_incidence_matrix_dense.h"

void signed_incidence_matrix_dense(
  const int n,
  const Eigen::MatrixXi & E,
  Eigen::MatrixXd & A)
{
    A = Eigen::MatrixXd::Zero(E.rows(), n);
    for (int r = 0; r < E.rows(); r++) {
        // By convention we use += instead of = to allow for repeated
        // (i, t) pairs in the rows list.
        A(r, E(r, 0)) += 1;
        A(r, E(r, 1)) += -1;
    }
}
