#include "fast_mass_springs_step_dense.h"
#include <igl/matlab_format.h>

void fast_mass_springs_step_dense(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXi & b,
  const double delta_t,
  const Eigen::MatrixXd & fext,
  const Eigen::VectorXd & r,
  const Eigen::MatrixXd & M,
  const Eigen::MatrixXd & A,
  const Eigen::MatrixXd & C,
  const Eigen::LLT<Eigen::MatrixXd> & prefactorization,
  const Eigen::MatrixXd & Uprev,
  const Eigen::MatrixXd & Ucur,
  Eigen::MatrixXd & Unext)
{
    // Conduct a single step of the "Fast Simulation of Mass-Spring Systems" method.
    //
    // Inputs: 
    //   V  #V by 3 list of **rest** vertex positions
    //   E  #E by 2 list of edge indices into rows of V
    //   k  spring stiffness
    //   b  #b list of indices of fixed vertices as indices into rows of V
    //   delta_t  time step in seconds
    //   fext  #V by 3 list of external forces
    //   r  #E list of edge lengths
    //   M  #V by #V mass matrix
    //   A  #E by #V signed incidence matrix
    //   C  #b by #V selection matrix
    //   prefactorization  LLT prefactorization of energy's quadratic matrix
    //   Uprev  #V by 3 list of previous vertex positions (at time t-∆t)
    //   Ucur  #V by 3 list of current vertex positions (at time t)
    // Outputs:
    //   Unext #V by 3 list of next vertex positions (at time t+∆t)

    Eigen::MatrixXd d(E.rows(), 3);
    // f is the force that is constant and does not depend on Unext,
    // so we compute it once ouside the loop.
    Eigen::MatrixXd f = (M / pow(delta_t, 2)) * (2 * Ucur - Uprev) + fext;
    double w = 1e10;
    for(int iter = 0; iter < 50; iter++)
    {
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
