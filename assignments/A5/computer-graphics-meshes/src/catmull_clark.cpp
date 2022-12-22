#include "catmull_clark.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include <map>
#include <vector>
#include <iostream>

typedef std::pair<int, int> pair;

std::map<pair, std::vector<int>> computeEdgesWithNeighbouringFaces(Eigen::MatrixXd &V, Eigen::MatrixXi &F, std::vector<pair> &edgePointsOrder) {
    std::map<pair, std::vector<int>> edgesToNeighbouringFacesMap;
    for (int f = 0; f < F.rows(); f++) {
        for (int i = 0; i < 4; i++) {
            pair edgeOneDirection;
            pair edgeOtherDirection;
            if (i == 3) {
                edgeOneDirection = std::make_pair(F.row(f)(3), F.row(f)(0));
                edgeOtherDirection = std::make_pair(F.row(f)(0), F.row(f)(3));
            } else {
                edgeOneDirection = std::make_pair(F.row(f)(i), F.row(f)(i + 1));
                edgeOtherDirection = std::make_pair(F.row(f)(i + 1), F.row(f)(i));
            }

            if (edgesToNeighbouringFacesMap.count(edgeOneDirection)) {
                edgesToNeighbouringFacesMap[edgeOneDirection].push_back(f);
            } else if (edgesToNeighbouringFacesMap.count(edgeOtherDirection)) {
                edgesToNeighbouringFacesMap[edgeOtherDirection].push_back(f);
            } else {
                std::vector<int> value;
                value.push_back(f);
                edgesToNeighbouringFacesMap.insert({edgeOneDirection, value});
                edgePointsOrder.push_back(edgeOneDirection);
            }

        }
    }
    return edgesToNeighbouringFacesMap;
}

std::map<int, std::vector<int>> computeVerticesWithNeighbouringFaces(Eigen::MatrixXd &V,
    Eigen::MatrixXi &F) {
    std::map<int, std::vector<int>> verticesToNeighbouringFacesMap;
    for (int f = 0; f < F.rows(); f++) {
        for (int i = 0; i < 4; i++) {
            int v = F.row(f)(i);
            if (verticesToNeighbouringFacesMap.count(v)) {
                verticesToNeighbouringFacesMap[v].push_back(f);
            } else {
                std::vector<int> value;
                value.push_back(f);
                verticesToNeighbouringFacesMap.insert({v, value});
            }
        }
    }
    return verticesToNeighbouringFacesMap;
}

Eigen::MatrixXi quadrupleFace(int face, Eigen::MatrixXi &F, std::map<int,
    int> facePointToVertexIndexMap, std::map<pair, int> edgePointToVertexIndexMap) {

    Eigen::MatrixXi newFaces = Eigen::MatrixXi::Zero(4, 4);
    for (int i = 0; i < 4; i++) {
        newFaces.row(i)(0) = F.row(face)(i);
        newFaces.row(i)(2) = facePointToVertexIndexMap[face];
    }
    for (int i = 0; i < 4; i++) {
        pair edge;
        if (i == 3) {
            edge = std::make_pair(F.row(face)(3), F.row(face)(0));
        } else {
            edge = std::make_pair(F.row(face)(i), F.row(face)(i + 1));
        }

        // We have saved one edge point per edge. Since two faces share an edge (a, b), 
        // we will have one saved only one of (a, b) or (b, a).
        int edgePointVertex = -1;
        if (edgePointToVertexIndexMap.count(edge)) {
            edgePointVertex = edgePointToVertexIndexMap[edge];
            for (int i = 0; i < 4; i++) {
                if (newFaces.row(i)(0) == edge.first) {
                    newFaces.row(i)(1) = edgePointVertex;
                }
                if (newFaces.row(i)(0) == edge.second) {
                    newFaces.row(i)(3) = edgePointVertex;
                }
            }
        } else {
            edgePointVertex = edgePointToVertexIndexMap[std::make_pair(edge.second, edge.first)];
        }

        for (int i = 0; i < 4; i++) {
            if (newFaces.row(i)(0) == edge.first) {
                newFaces.row(i)(1) = edgePointVertex;
            }
            if (newFaces.row(i)(0) == edge.second) {
                newFaces.row(i)(3) = edgePointVertex;
            }
        }
    }
    return newFaces;
}

void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
    //   SV  #SV by 3 list of vertex positions
    //   SF  #SF by 4 list of quad mesh indices into SV

    Eigen::MatrixXd startingVertices = V;
    Eigen::MatrixXi startingFaces = F;

    // Run the "Catmull–Clark surfaces" num_iters times
    for (int iter = 0; iter < num_iters; iter++) {
        int numVerticesSoFar = startingVertices.rows();
        std::vector<pair> edgePointsOrder;
        std::map<pair, std::vector<int>> edgesToNeighbouringFacesMap = computeEdgesWithNeighbouringFaces(startingVertices, startingFaces, edgePointsOrder);
        std::map<int, std::vector<int>> verticesToNeighbouringFacesMap = computeVerticesWithNeighbouringFaces(startingVertices, startingFaces);

        // The number of new vertices is old vertices + one face point per face + one edge point per edge.
        SV.resize(startingVertices.rows() + startingFaces.rows() + edgesToNeighbouringFacesMap.size(), 3);

        std::map<int, int> facePointToVertexIndexMap;
        std::map<pair, int> edgePointToVertexIndexMap;

        // 1. For each face, add a face point by setting each face point
        // to be the average of all original points for the respective face
        Eigen::MatrixXd facePoints = Eigen::MatrixXd::Zero(startingFaces.rows(), 3);
        // numFaceVertices is guranteed to be 4, since we have a pure quad mesh.
        int numFaceVertices = startingFaces.cols();
        for (int f = 0; f < startingFaces.rows(); f++) {
            Eigen::RowVector3d average = Eigen::MatrixXd::Zero(1, 3);
            for (int v = 0; v < numFaceVertices; v++) {
                average += startingVertices.row(startingFaces.row(f)(v));
            }
            facePoints.row(f) << average / numFaceVertices;
            facePointToVertexIndexMap.insert({f, numVerticesSoFar});
            SV.row(numVerticesSoFar) << average / numFaceVertices;
            numVerticesSoFar++;
        }

        // 2. For each edge, add an edge point by setting each edge point
        // to be the average of the two neighbouring face points and
        // the midpoint of the edge
        for (int i = 0; i < edgePointsOrder.size(); i++) {
            pair edge = edgePointsOrder[i];
            // pair edge = it->first;
            std::vector<int> faces = edgesToNeighbouringFacesMap[edge];
            Eigen::RowVector3d facePointsSum = facePoints.row(faces[0]) + facePoints.row(faces[1]);
            Eigen::RowVector3d edgeVerticesAverage = (startingVertices.row(edge.first) + startingVertices.row(edge.second)) / 2.0;
            edgePointToVertexIndexMap.insert({edge, numVerticesSoFar});
            SV.row(numVerticesSoFar) << (facePointsSum + edgeVerticesAverage) / 3.0;
            numVerticesSoFar++;
        }

        // 3. For each original point p, let facePointsAverage be the average of all n
        // (recently created) face points for faces touching p.
        // let originalEdgeMidPointsAverage be the average of all n
        // edge midpoints for original edges touching p.
        for (int vertex = 0; vertex < startingVertices.rows(); vertex++) {
            int numNeighbouringFaces = verticesToNeighbouringFacesMap[vertex].size();
            Eigen::RowVector3d facePointsAverage = Eigen::MatrixXd::Zero(1, 3);
            Eigen::RowVector3d originalEdgeMidPointsAverage = Eigen::MatrixXd::Zero(1, 3);
            for (int i = 0; i < numNeighbouringFaces; i++) {
                int f = verticesToNeighbouringFacesMap[vertex][i];
                facePointsAverage += facePoints.row(f);
                //
                for (int v = 0; v < 4; v++) {
                    if (startingFaces.row(f)(v) == vertex) {
                        if (v == 0) {
                            // last vertex and first vertex are the edge touching the vertex
                            originalEdgeMidPointsAverage += (startingVertices.row(startingFaces.row(f)(3)) + startingVertices.row(startingFaces.row(f)(0))) / 2.0;
                        } else {
                            // v and v-1 are the edge touching the vertex
                            originalEdgeMidPointsAverage += (startingVertices.row(startingFaces.row(f)(v)) + startingVertices.row(startingFaces.row(f)(v-1))) / 2.0;
                        }
                    }
                }
            }
            facePointsAverage = facePointsAverage / numNeighbouringFaces;
            originalEdgeMidPointsAverage = originalEdgeMidPointsAverage / numNeighbouringFaces;
            SV.row(vertex) << (facePointsAverage + 2 * originalEdgeMidPointsAverage + (numNeighbouringFaces - 3) * startingVertices.row(vertex)) / numNeighbouringFaces;
        }

        int newFacesIndex = 0;
        SF.resize(startingFaces.rows() * 4, 4);
        for (int f = 0; f < startingFaces.rows(); f++) {
            Eigen::MatrixXi newFaces = quadrupleFace(f, startingFaces, facePointToVertexIndexMap, edgePointToVertexIndexMap);
            // std::cout << "face " << f << " ....\n";
            for (int i = 0; i < 4; i++) {
                // std::cout << newFaces.row(i)  << " row \n";
                SF.row(newFacesIndex) = newFaces.row(i);
                newFacesIndex++;
            }
        }

        // Update new startingVertices and startingFaces
        startingVertices.resize(SV.rows(), 3);
        startingVertices = SV;
        startingFaces.resize(SF.rows(), 4);
        startingFaces = SF;
    }
}
