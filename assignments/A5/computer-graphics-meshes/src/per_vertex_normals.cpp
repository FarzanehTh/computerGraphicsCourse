#include "per_vertex_normals.h"
#include "triangle_area_normal.h"
#include "vertex_triangle_adjacency.h"
#include "per_face_normals.h"

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
    N = Eigen::MatrixXd::Zero(V.rows(),3);

    std::vector<std::vector<int>> verticesAdjacentFaces;
    vertex_triangle_adjacency(F, V.rows(), verticesAdjacentFaces);

    Eigen::MatrixXd perFaceNormals;
    per_face_normals(V, F, perFaceNormals);

    for (int v = 0; v < V.rows(); v++) {
        // Compute the area weighted average of the vertex v
        // by finding all the faces that it belongs to.
        Eigen::RowVector3d vertexNormal = Eigen::MatrixXd::Zero(1,3);
        for (int i = 0; i < verticesAdjacentFaces[v].size(); i++) {
            int f = verticesAdjacentFaces[v][i];
            int a = F.row(f)(0);
            int b = F.row(f)(1);
            int c = F.row(f)(2);
            Eigen::RowVector3d faceAreaVector = triangle_area_normal(V.row(a), V.row(b), V.row(c));
            Eigen::RowVector3d faceNormal = perFaceNormals.row(f);

            vertexNormal += (faceAreaVector.array() * faceNormal.array()).matrix();
        }
        vertexNormal = vertexNormal / vertexNormal.norm();
        N.row(v) << vertexNormal;
    }
}
