#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include "per_face_normals.h"
#include <iostream>
#include <math.h>

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
    N = Eigen::MatrixXd::Zero(F.rows()*3, 3);

    // Compute per corner normals for a triangle mesh by computing the area-weighted
    // average of normals at incident faces whose normals deviate less than the
    // provided threshold.

    std::vector<std::vector<int>> verticesAdjacentFaces;
    vertex_triangle_adjacency(F, V.rows(), verticesAdjacentFaces);

    Eigen::MatrixXd perFaceNormals;
    per_face_normals(V, F, perFaceNormals);

    for (int face = 0; face < F.rows(); face++) {
        Eigen::RowVector3d curCornerFaceNormal = perFaceNormals.row(face);
        for (int i = 0; i < F.cols(); i++) {
            int v = F.row(face)(i);
            Eigen::RowVector3d cornerNormal = Eigen::MatrixXd::Zero(1, 3);
            for (int t = 0; t < verticesAdjacentFaces[v].size(); t++) {
                int f = verticesAdjacentFaces[v][t];
                int a = F.row(f)(0);
                int b = F.row(f)(1);
                int c = F.row(f)(2);
                Eigen::RowVector3d faceAreaVector = triangle_area_normal(V.row(a), V.row(b), V.row(c));
                Eigen::RowVector3d faceNormal = perFaceNormals.row(f);
                // normalsDeviation is equal to the cos of the angle between the faceNormal and curCornerFaceNormal
                double normalsDeviation = faceNormal.dot(curCornerFaceNormal) / (faceNormal.norm() * curCornerFaceNormal.norm());
                // The angle between the faceNormal and curCornerFaceNormal should be less that or equal to
                // corner_threshold, which means the cos of their angle should be greater than
                // the cos of corner_threshold. (Note the "corner_threshold" is in "degrees")
                if (normalsDeviation > cos(corner_threshold)) {
                    cornerNormal += (faceAreaVector.array() * faceNormal.array()).matrix();
                }
            }
            cornerNormal = cornerNormal / cornerNormal.norm();
            N.row(face * 3 + i) << cornerNormal;
        }
    }
}
