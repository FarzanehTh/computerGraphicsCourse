#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
  const std::string & filename,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & UV,
  const Eigen::MatrixXi & UF,
  const Eigen::MatrixXd & NV,
  const Eigen::MatrixXi & NF)
{
  assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");

  // I used https://en.wikipedia.org/wiki/Wavefront_.obj_file for this function
  std::ofstream outputFile;
  outputFile.open(filename);
  if (outputFile.is_open()) {
    // List of geometric vertices, with (x, y, z, [w]) coordinates, w is optional and defaults to 1.0.
    outputFile << "# List of geometric vertices, with (x, y, z, [w]) coordinates, w is optional and defaults to 1.0. \n";
    for (int r = 0; r < V.rows(); r++) {
        outputFile << "v " + std::to_string(V.row(r)(0)) + " " + std::to_string(V.row(r)(1)) + " " + std::to_string(V.row(r)(2)) + "\n";
    }

    // List of texture coordinates (i.e. UV positions)
    outputFile << "# List of texture coordinates, in (u, [v, w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0. \n";
    for (int r = 0; r < UV.rows(); r++) {
        outputFile << "vt " + std::to_string(UV.row(r)(0)) + " " + std::to_string(UV.row(r)(1)) + "\n";
    }

    // List of vertex normals in (x,y,z) form; normals might not be unit vectors.
    outputFile << "# List of vertex normals in (x,y,z) form; normals might not be unit vectors. \n";
    for (int r = 0; r < NV.rows(); r++) {
        outputFile << "vn " + std::to_string(NV.row(r)(0)) + " " + std::to_string(NV.row(r)(1)) + " " + std::to_string(NV.row(r)(2)) + "\n";
    }

    // Polygonal face element
    int numCols = F.cols();
    outputFile << "# Polygonal face element as list of indexes of their vertex_index/texture_index/normal_index. \n";
    for (int r = 0; r < F.rows(); r++) {
        // Write the indexes of vertex, texture and normal indices
        // in the form of "vertex_index/texture_index/normal_index"
        outputFile << "f ";
        for (int c = 0; c < numCols; c++) {
            outputFile << std::to_string((F.row(r)(c) + 1)) + "/" + std::to_string((UF.row(r)(c) + 1)) + "/" + std::to_string((NF.row(r)(c) + 1)) + " ";
        }
        outputFile << "\n";
    }
    outputFile.close();
    return true;
  }
  return false;
}
