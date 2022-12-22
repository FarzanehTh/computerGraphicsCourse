#include "forward_kinematics.h"
#include "euler_angles_to_transform.h"
#include <functional> // std::function
#include <set>
#include <list>

Eigen::Affine3d compute_affine_transformations(const Skeleton & skeleton, std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d>>&T, int index, std::vector<bool> &visited) {
    
    Eigen::Affine3d affineTransformation = Eigen::Affine3d::Identity();
    Bone bone = skeleton[index];
    Eigen::Affine3d rotationTransformation = euler_angles_to_transform(bone.xzx);
    Eigen::Affine3d parentTransformation;
    if (bone.parent_index == -1) {
        parentTransformation = Eigen::Affine3d::Identity();
    } else {
        parentTransformation = visited[bone.parent_index] ? T[bone.parent_index] : compute_affine_transformations(skeleton, T, bone.parent_index, visited);
    }
    // T = T_p * T_rest * rotation_in_canonical_space * T_rest_reverse
    affineTransformation = parentTransformation * skeleton[index].rest_T * rotationTransformation * skeleton[index].rest_T.inverse();

    visited[index] = true;
    T[index] = affineTransformation;
    return affineTransformation;
}

void forward_kinematics(
  const Skeleton & skeleton,
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T)
{
    T.resize(skeleton.size(),Eigen::Affine3d::Identity());
    std::vector<bool> visited(skeleton.size());
    for (int i = 0; i < skeleton.size(); i++) {
        if (!visited[i]) {
            // This bone have not been visited yet
            compute_affine_transformations(skeleton, T, i, visited);
        }
    }
}
