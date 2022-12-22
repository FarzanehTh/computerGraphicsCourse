#include "per_face_normals.h"
#include "triangle_area_normal.h"

void per_face_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
    // N  #F by 3 list of per-face unit normal vectors
    N = Eigen::MatrixXd::Zero(F.rows(), 3);
    for (int f = 0; f < F.rows(); f++) {
        int a = F.row(f)(0);
        int b = F.row(f)(1);
        int c = F.row(f)(2);
        N.row(f) << triangle_area_normal(V.row(a), V.row(b), V.row(c));
    }
}
