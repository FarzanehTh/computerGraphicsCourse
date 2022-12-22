#include "linear_blend_skinning.h"

void linear_blend_skinning(
  const Eigen::MatrixXd & V,
  const Skeleton & skeleton,
  const std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T,
  const Eigen::MatrixXd & W,
  Eigen::MatrixXd & U)
{
    U.resize(V.rows(), 3);
    for (int i = 0; i < V.rows(); i++) {
        Eigen::Vector4d transformedV = Eigen::Vector4d::Zero(4);
        for (int b = 0; b < skeleton.size(); b++) {
            int weight = skeleton[b].weight_index;
            if (weight != -1) {
                Eigen::Vector4d restV;
                restV << V.row(i)(0), V.row(i)(1), V.row(i)(2), 1;
                if (W(i, weight) != -1) {
                    transformedV += W(i, weight) * (T[b] * restV);
                }
            }
        }
        U.row(i) << transformedV(0), transformedV(1), transformedV(2);
    }
}
