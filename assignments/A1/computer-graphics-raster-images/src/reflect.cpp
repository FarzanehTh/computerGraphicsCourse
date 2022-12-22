#include "reflect.h"

void reflect(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & reflected)
{
    reflected.resize(width*height*num_channels);

    // To reflect horizontally like a mirror, given a pixel has the dim (row1, col1),
    // the reflection of this pixel will be at the pixel (row2, col2), such that
    // col1 == col2 but the rows will be different. Given the row1, the row2
    // will be (height - 1) - row1. We need to go over the input image and
    // for do this reflection for the top half of height of it.
    for(int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int i = (row * width + col) * num_channels;
            int r = (row * width + (width - 1 - col)) * num_channels;
            if (num_channels == 3) {
                reflected[i] = input[r];
                reflected[i+1] = input[r+1];
                reflected[i+2] = input[r+2];

                reflected[r] = input[i];
                reflected[r+1] = input[i+1];
                reflected[r+2] = input[i+2];
            } else if (num_channels == 1) {
                reflected[i] = input[r];
                reflected[r] = input[i];
            }
        }
    }
}
