#include "rotate.h"

void rotate(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & rotated)
{
    rotated.resize(height*width*num_channels);

    for (unsigned int row = 0; row < height; row++) {
        for (unsigned int col = 0; col < width; col++) {
            // i is the current index of the input image that we are looking at
            unsigned i = (row * width + col) * num_channels;
            unsigned newRow = width - 1 - col;
            unsigned newCol = row;
            // r is the i's corresponding index in the rotated image
            unsigned r = (newRow * height + newCol) * num_channels;
            if (num_channels == 3) {
                // We want to transfer all 3 RGB values together
                rotated[r] = input[i];
                rotated[r+1] = input[i+1];
                rotated[r+2] = input[i+2];
            } else if (num_channels == 1) {
                rotated[r] = input[i];
            }
        }
    }
}
