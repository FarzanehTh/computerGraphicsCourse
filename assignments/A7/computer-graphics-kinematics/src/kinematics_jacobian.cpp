#include "kinematics_jacobian.h"
#include "copy_skeleton_at.h"
#include "transformed_tips.h"
#include <iostream>

void kinematics_jacobian(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  Eigen::MatrixXd & J)
{
    // b  #b list of indices into skeleton of endpoints to consider (i.e. list of end-effectors)
    double h = pow(10, -7);
    J = Eigen::MatrixXd::Zero(b.size() * 3, skeleton.size() * 3);

    Eigen::VectorXd originalValue(skeleton.size() * 3);
    for (int boneIndx = 0; boneIndx < skeleton.size(); boneIndx++) {
        originalValue(boneIndx * 3) = skeleton[boneIndx].xzx(0);
        originalValue(boneIndx * 3 + 1) = skeleton[boneIndx].xzx(1);
        originalValue(boneIndx * 3 + 2) = skeleton[boneIndx].xzx(2);
    }

    for (int i = 0; i < b.size() * 3; i++) {
        for (int t = 0; t < skeleton.size() * 3; t++) {
            // Change t_th angle of the bone boneIndx by a small h amount
            int boneIndx = t / 3;
            Eigen::VectorXd sigma = Eigen::VectorXd::Zero(skeleton.size() * 3);
            sigma(t) = 1;

            // Change the bone boneIndx by adding h * sigma
            Skeleton skeletonCopyPlus = copy_skeleton_at( skeleton, originalValue + h * sigma);
            Eigen::VectorXd transformedTipsPlus = transformed_tips(skeletonCopyPlus, b);

            // Change the bone boneIndx by subtracting h * sigma
            Skeleton skeletonCopyMinus = copy_skeleton_at(skeleton, originalValue - h * sigma);
            Eigen::VectorXd transformedTipsMinus = transformed_tips(skeletonCopyMinus, b);

            J(i, t) = (transformedTipsPlus((i / 3) + (i % 3)) - transformedTipsMinus((i / 3) + (i % 3))) / 2 * h;
        }
    }
}
