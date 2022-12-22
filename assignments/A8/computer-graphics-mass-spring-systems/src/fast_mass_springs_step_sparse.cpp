#include "fast_mass_springs_step_sparse.h"
#include <igl/matlab_format.h>

void fast_mass_springs_step_sparse(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXi & b,
  const double delta_t,
  const Eigen::MatrixXd & fext,
  const Eigen::VectorXd & r,
  const Eigen::SparseMatrix<double>  & M,
  const Eigen::SparseMatrix<double>  & A,
  const Eigen::SparseMatrix<double>  & C,
  const Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > & prefactorization,
  const Eigen::MatrixXd & Uprev,
  const Eigen::MatrixXd & Ucur,
  Eigen::MatrixXd & Unext)
{
    Eigen::MatrixXd d(E.rows(), 3);
    
    // f is the force that is constant and does not depend on Unext,
    // so we compute it once ouside the loop.
    Eigen::MatrixXd f = (M / pow(delta_t, 2)) * (2 * Ucur - Uprev) + fext;
    double w = 1e10;
    for (int iter = 0; iter < 50; iter++) {
        // Local optimization: given the currect Unext value for vertices,
        // find the d values (i.e. rest position) for each of them.
        for (int i = 0; i < E.rows(); i++) {
            // We want the optimal d to have the same direction as Unext
            // positions but have the length equal to the original length r(i).
            // We chose the rest vectors r to be always from E(i, 1) to E(i, 0).
            // So we should make sure we are consistent here.
            d.row(i) << (Unext.row(E(i, 0)) - Unext.row(E(i, 1))).normalized() * r(i);
        }

        // Global optimization
        // Add the penalty to minimize considreing the pinned vertices
        Eigen::MatrixXd penalty = w * C.transpose() * C * V;
        Eigen::MatrixXd l = k * A.transpose() * d + f + penalty;
        Unext = prefactorization.solve(l);
    }
}
