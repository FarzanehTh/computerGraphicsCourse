#include "fast_mass_springs_precomputation_dense.h"
#include "signed_incidence_matrix_dense.h"
#include <Eigen/Dense>
#include <iostream>

bool fast_mass_springs_precomputation_dense(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXd & m,
  const Eigen::VectorXi & b,
  const double delta_t,
  Eigen::VectorXd & r,
  Eigen::MatrixXd & M,
  Eigen::MatrixXd & A,
  Eigen::MatrixXd & C,
  Eigen::LLT<Eigen::MatrixXd> & prefactorization)
{
    // Precompute matrices and factorizations necessary for the "Fast Simulation of
    // Mass-Spring Systems" method.
    //
    // Inputs: 
    //   V  #V by 3 list of vertex positions
    //   E  #E by 2 list of edge indices into rows of V
    //   k  spring stiffness
    //   m  #V list of masses 
    //   b  #b list of "pinned"/fixed vertices as indices into rows of V
    //   delta_t  time step in seconds
    // Outputs:
    //   r  #E list of edge lengths
    //   M  #V by #V mass matrix
    //   A  #E by #V signed incidence matrix
    //   C  #b by #V selection matrix
    //   prefactorization  LLT prefactorization of energy's quadratic matrix

    // Make r
    r.resize(E.rows());
    for (int i = 0; i < E.rows(); i++) {
        // We chose the rest vectors r to be always from E(i, 1) to E(i, 0).
        r(i) = (V.row(E(i, 0)) - V.row(E(i, 1))).norm();
    }

    // Make M
    M = Eigen::MatrixXd::Zero(V.rows(), V.rows());
    for (int i = 0; i < V.rows(); i++) {
        M(i, i) = m(i);
    }

    // Make A
    signed_incidence_matrix_dense(V.rows(), E, A);

    // Make C
    C = Eigen::MatrixXd::Zero(b.rows(), V.rows());
    for (int i = 0; i < b.rows() ; i++) {
        int index = b(i);
        C(i, index) = 1;
    }

    // Apply prefactorization
    double w = 1e10;
    // Compute Q along with the penalty so that we minimize considering the pinned vertices.
    Eigen::MatrixXd penalty = w * C.transpose() * C;
    Eigen::MatrixXd Q = k * A.transpose() * A + (M / pow(delta_t, 2)) + penalty;

    prefactorization.compute(Q);
    return prefactorization.info() != Eigen::NumericalIssue;
}
