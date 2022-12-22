#include "copy_skeleton_at.h"
#include <iostream>

Skeleton copy_skeleton_at(
  const Skeleton & skeleton, 
  const Eigen::VectorXd & A)
{
    Skeleton copy;
    for (int b = 0; b < skeleton.size(); b++) {
        Bone bone = skeleton[b];
        bone.xzx(0) = A(b * 3 + 0);
        bone.xzx(1) = A(b * 3 + 1);
        bone.xzx(2) = A(b * 3 + 2);
        copy.push_back(bone);
    }
    return copy;
}
