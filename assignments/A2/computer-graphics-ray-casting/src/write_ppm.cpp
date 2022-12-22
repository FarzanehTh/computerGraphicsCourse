#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  assert((num_channels == 3 || num_channels == 1) && ".ppm only supports RGB or grayscale images");

  // I used https://en.wikipedia.org/wiki/Netpbm#PPM_example for this function
  std::ofstream outputFile;
  outputFile.open(filename);
  if (outputFile.is_open()) {
      // "P3" means this is a RGB color image in ASCII and
      // "P2" means this is a grayscale image in ASCII
      if (num_channels == 3) {
          outputFile << "P3\n";
      } else {
          outputFile << "P2\n";
      }
      // Write the width and height of the image in pixels
      outputFile << std::to_string(width) + " " + std::to_string(height) + "\n";
      // "255" is the maximum value for each color
      outputFile << "255\n";
      // Now write the image data: RGB triplets or grayscale single values
      for (int row = 0; row < height; row++) {
          for (int col = 0; col < width; col++) {
              int i = row * width + col;
              if (num_channels == 3) {
                  i = i * 3;
                  outputFile << std::to_string(data[i]) + " " + std::to_string(data[i + 1]) + " " + std::to_string(data[i + 2]) + "\n";
              } else if (num_channels == 1) {
                  outputFile << std::to_string(data[i]) + "\n";
              }
          }
      }
      outputFile.close();
      return true;
  }
  return false;
}
