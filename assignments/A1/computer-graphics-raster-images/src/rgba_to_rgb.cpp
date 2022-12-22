#include "rgba_to_rgb.h"

void rgba_to_rgb(
  const std::vector<unsigned char> & rgba,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
    rgb.resize(height * width * 3);

    // We do not want the 4th info of each pixel. i.e. its alpha value
    // Given that the image is stored as a 1-D array of bytes, that means
    // we do not want the elements at indexes 3, 7, 11, ..., when
    // enumerating the vector rgba from i = 0. So we should append
    // rgba's elements to the rgb except the ones at indexes 3, 7, 11, ...
    // unsigned int rgbaImageSize = height * width * 4;
    // unsigned int alphaIndex = 3;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int i = (row * width + col) * 4;
            // Since rgba contains 4 elements and rgb contains 3 elements per pixel,
            // we need to adjust the indexes. So here we use the i for rgba and the r for rgb.
            int r = i == 0 ? i : i - (row * width + col);
            rgb[r] = rgba[i];
            rgb[r + 1] = rgba[i + 1];
            rgb[r + 2] = rgba[i + 2];
        }
    }
}
