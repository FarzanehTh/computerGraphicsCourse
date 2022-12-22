#include "rgb_to_gray.h"

void rgb_to_gray(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  std::vector<unsigned char> & gray)
{
    gray.resize(height*width);

    for (int i = 0; i < width * height; i++) {
        gray[i] = (unsigned char)(0.2126 * (float)rgb[3 * i] + 0.7152 * (float)rgb[3 * i + 1] + 0.0722 * (float)rgb[3 * i + 2]);
    }
}


