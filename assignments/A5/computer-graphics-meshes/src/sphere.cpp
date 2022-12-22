#include "sphere.h"
#include <iostream>
#include <math.h>
#include <Eigen/Geometry>

void sphere(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
    // Inputs:
    //   num_faces_u  number of faces in the longitudinal direction
    //   num_faces_v  number of faces in the latitudinal direction
    // Outputs:
    //   V  #V by 3 list of 3D corner vertex positions
    //   F  #F by 4 list of quad face indices into rows of V
    //   UV  #UV by 2 list of corner parameterization positions
    //   UF  #F by 4 list of quad face indices into rows of UV
    //   NV  #NV  by 3 list of 3D unit normal vectors
    //   NF  #F by 4 list of quad face indices into rows of NV

    int numFaces = num_faces_u * num_faces_v;
    int numVertices = (num_faces_u + 1) * (num_faces_v + 1);
    V.resize(numVertices, 3);
    NV.resize(numVertices, 3);

    // M_PI is equal to pi.
    double theta = M_PI / num_faces_u;
    double phi = (2 * M_PI) / num_faces_v;
    for (int u = 0; u <= num_faces_u; u++) {
        for (int v = 0; v <= num_faces_v; v++) {
            // The theta at the north pole will all have theta value of 0.
            // This means they will all be mapped to (0, 0, 1) and so 
            // there will not be a hole. 
            // By the same reasoning, the north pole vertices will all
            // be mapped to (0, 0, -1), since theta will be -pi.
            // So we do not need to be worried about having a hole at the top or bottom.
            int index = u * (num_faces_v + 1) + v;
            double x = sin(theta * u) * cos(phi * v);
            double y = sin(theta * u) * sin(phi * v);
            double z = cos(theta * u);
            V.row(index) << x, y, z;
            // The vector (x, y, z) is already normal to the plane at that point.
            NV.row(index) << x, y, z;
        }
    }

    // In order to have a quad around the poles, we re-use the pole's vertices
    // two times for each face around it, to make sure all faces are quads.
    // Also make sure the order of vertices for faces are counterclockwise.
    F.resize(numFaces, 4);
    for (int u = 0; u < num_faces_u; u++) {
        for (int v = 0; v < num_faces_v; v++) {
            // Create faces by considering the vertices counter-clockwise
            int v1 = u * (num_faces_v + 1) + v;
            int v2 = (u + 1) * (num_faces_v + 1) + v;
            int v3 = (u + 1) * (num_faces_v + 1) + (v + 1);
            int v4 = u * (num_faces_v + 1) + (v + 1);
            // Note that because we used the sphere parametrization of theta and phi above,
            // the vertices of the faces that touch each other are diffrent vertices by index,
            // but their values are actually the same. So we will be ok here and do not need
            // to take care of the faces at the edges by a different logic.
            F.row(u * num_faces_v + v) << v1, v2, v3, v4;
        }
    }

    // We will have one UV parameterization per vertex on a
    // 1 by 1 square divided up by num_faces_u along the logitudinal
    // direction and num_faces_v along the latitudinal direction.
    UV.resize((num_faces_u + 1) * (num_faces_v + 1), 2);
    double uIncrement = (double)(1.0 / (num_faces_u + 1));
    double vIncrement = (double)(1.0 / (num_faces_v + 1));
    for (int i = 0; i < num_faces_u + 1; i++) {
        for (int t = 0; t < num_faces_v + 1; t++) {
            UV.row(i * (num_faces_v + 1) + t) << t * vIncrement, i * uIncrement;
        }
    }

    // Parameterization and normal vectors of each vertex is exactly
    // at the same index that its face is referring to.
    UF.resize(numFaces, 4);
    NF.resize(numFaces, 4);
    for (int f = 0; f < numFaces; f++) {
        UF.row(f) << F.row(f);
        NF.row(f) << F.row(f);
    }
}
