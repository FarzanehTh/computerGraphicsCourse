#include "transformed_tips.h"
#include "forward_kinematics.h"
#include <iostream>

Eigen::VectorXd transformed_tips(
  const Skeleton & skeleton, 
  const Eigen::VectorXi & b)
{
    std::vector<Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d>> T;
    forward_kinematics(skeleton, T);

    // Returns  #b*3 vector of transformed tip positions 
    Eigen::VectorXd boneTipsTransformed = Eigen::VectorXd::Zero(3 * b.size());
    std::vector<bool> visited(skeleton.size());
    std::vector<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d>> D(skeleton.size());

    for (int i = 0; i < b.size(); i++) {
        Bone bone = skeleton[b[i]];
        // T = T_p * T_rest * rotation_in_canonical_space * T_rest_reverse
        Eigen::Vector4d boneTip(bone.length, 0, 0, 1);
        Eigen::Vector4d d = (T[b[i]] * bone.rest_T) * boneTip;

        boneTipsTransformed(i * 3) = d(0);
        boneTipsTransformed(i * 3 + 1) = d(1);
        boneTipsTransformed(i * 3 + 2) = d(2);
    }
    return boneTipsTransformed;
}
