#include "end_effectors_objective_and_gradient.h"
#include "transformed_tips.h"
#include "kinematics_jacobian.h"
#include "copy_skeleton_at.h"
#include <iostream>

void end_effectors_objective_and_gradient(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  const Eigen::VectorXd & xb0,
  std::function<double(const Eigen::VectorXd &)> & f,
  std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  std::function<void(Eigen::VectorXd &)> & proj_z)
{
    // f function handle that computes the least-squares objective value given a
    // #bones list of Euler angles 
    f = [&](const Eigen::VectorXd & A)->double
    {   
        Skeleton transformedSkeleton = copy_skeleton_at(skeleton, A);
        // b #b list of indices into skelton of endpoints to compute
        // #b*3 vector of transformed tip positions
        Eigen::VectorXd xbi = transformed_tips(transformedSkeleton, b);
        return 0.5 * pow((xbi - xb0).norm(), 2);
    };

    // grad_f function handle that computes the least-squares objective gradient
    // given a #bones list of Euler angles 
    grad_f = [&](const Eigen::VectorXd & A)->Eigen::VectorXd
    {   
        // The angles A is the thing that we are trying to optimize
        Skeleton transformedSkeleton = copy_skeleton_at(skeleton, A);
        // b #b list of indices into skelton of endpoints to compute
        // #b*3 vector of transformed tip positions
        Eigen::VectorXd xbi = transformed_tips(transformedSkeleton, b);

        // The dimension of the derivativeE is (end_effectors * 3) * 1.
        Eigen::VectorXd derivativeE;
        derivativeE = xbi - xb0;

        // The dimension of the J is (end_effectors * 3) * (all_bones * 3).
        Eigen::MatrixXd J;
        kinematics_jacobian(skeleton, b, J);

        // The dimension of the gradiant is (all_bones * 3) * 1.
        return J.transpose() * derivativeE;
    };

    // proj_z function handle that projects a given set of Euler angles onto the
    // input skeleton's joint angles
    proj_z = [&](Eigen::VectorXd & A)
    {
        assert(skeleton.size()*3 == A.size());
        for (int i = 0; i < skeleton.size(); i++) {
            for (int t = 0; t < 3; t++) {
                A(i * 3 + t) = std::max(skeleton[i].xzx_min(t), std::min(skeleton[i].xzx_max(t), A(i * 3 + t)));
            }
        }
    };
}
