#include "cube.h"

void cube(
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
    // A cube has 8 vertices, 6 faces (each face having 4 vertices). 
    // Also in a 2-D format, it has 14 corners.
    V.resize(8,3);
    F.resize(6,4);
    UV.resize(14,2);
    UF.resize(6,4);
    NV.resize(6,3);
    NF.resize(6,4);

    // A cube has 8 vertices
    V << 1, -1, -1,
        1, 1, -1,
        1, 1, 1,
        1, -1, 1,
        -1, -1, -1,
        -1, 1, -1,
        -1, 1, 1,
        -1, -1, 1;

    // The order of the vertices of each quad face should be such that
    // we iterate them counter-clockwise, if we look at the face from outside;
    // and clockwise, if we look at it from inside.
    // The order of faces are:
    // Right(+x) which is blue,
    // Left(-x) which is red,
    // Back(+y) which is yellow,
    // Front(-y) which is white,
    // Top(+z) which is green,
    // Bottom(-z) which is orange
    F << 0, 1, 2, 3,
        5, 4, 7, 6,
        5, 6, 2, 1,
        0, 3, 7, 4,
        6, 7, 3, 2,
        5, 1, 0, 4;//

    // UV values can be obtained by drawing a cube expanded on a uv plane
    // such that u and v both range from 0 to 1 and the incrementation of 0.25.
    UV << 0.75, 0.25, 
        0.75, 0.5,
        0.5, 0.5,
        0.5, 0.25, 
        0, 0.5, 
        0, 0.25,
        0.25, 0.25,
        0.25, 0.5,
        1, 0.25,
        1, 0.5,
        0.5, 0.75,
        0.25, 0.75,
        0.5, 0,
        0.25, 0;

    // Indexes of vertices of each face into the UV
    UF << 0, 1, 2, 3,
        4, 5, 6, 7,
        11, 7, 2, 10,
        12, 3, 6, 13,
        7, 6, 3, 2,
        9, 1, 0, 8;

    // NV is the list of normals per face
    NV << 1, 0, 0,
        -1, 0, 0,
        0, 1, 0,
        0, -1, 0,
        0, 0, 1,
        0, 0, -1;

    // NF is indices of each vertices of faces into NV
    // For now all vertices of a face refer to the same normal based on the face (i.e. per-face normals).
    NF << 0, 0, 0, 0,
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4,
        5, 5, 5, 5;
}
