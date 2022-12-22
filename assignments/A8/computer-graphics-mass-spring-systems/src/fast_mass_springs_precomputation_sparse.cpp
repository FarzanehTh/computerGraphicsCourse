#include "fast_mass_springs_precomputation_sparse.h"
#include "signed_incidence_matrix_sparse.h"
#include <vector>

bool fast_mass_springs_precomputation_sparse(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXd & m,
  const Eigen::VectorXi & b,
  const double delta_t,
  Eigen::VectorXd & r,
  Eigen::SparseMatrix<double>  & M,
  Eigen::SparseMatrix<double>  & A,
  Eigen::SparseMatrix<double>  & C,
  Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > & prefactorization)
{
    int n = V.rows();

    // Make sparse matrix r
    r.resize(E.rows());
    for (int i = 0; i < E.rows(); i++) {
        // We chose the rest vectors r to be always from E(i, 1) to E(i, 0).
        r(i) = (V.row(E(i, 0)) - V.row(E(i, 1))).norm();
    }

    // Make sparse matrix M
    M.resize(n, n);
    std::vector<Eigen::Triplet<double>> ijvForM(n);
    for(int i = 0; i < n; i++) {
        ijvForM.emplace_back(i, i, m(i));
    }
    M.setFromTriplets(ijvForM.begin(), ijvForM.end());

    // Make sparse matrix A
    signed_incidence_matrix_sparse(n, E, A);

    // Make sparse matrix C
    C.resize(b.rows(), n);
    std::vector<Eigen::Triplet<double>> ijvForC(b.rows());
    for(int i = 0; i < b.rows(); i++) {
        ijvForC.emplace_back(i, b(i), 1);
    }
    C.setFromTriplets(ijvForC.begin(), ijvForC.end());

    // Apply prefactorization
    double w = 1e10;
    // Compute Q along with the penalty so that we minimize considering the pinned vertices.
    Eigen::SparseMatrix<double> penalty(n, n);
    penalty = w * C.transpose() * C;
    Eigen::SparseMatrix<double> Q(n, n);
    Q = k * A.transpose() * A + (M / pow(delta_t, 2)) + penalty;

    prefactorization.compute(Q);
    return prefactorization.info() != Eigen::NumericalIssue;
}
