#include "hue_shift.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void hue_shift(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double shift,
  std::vector<unsigned char> & shifted)
{
    shifted.resize(rgb.size());

    // When we want to work with rgb and hsv and turn them to each other, the rgb values should be
    // between 0-255. So we have to divide by 255.
    // Turn the rgb image to hsv image
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int i = (row * width + col) * 3;
            double r = rgb[i] / 255.0;
            double g = rgb[i + 1] / 255.0;
            double b = rgb[i + 2] / 255.0;
            double h = 0;
            double s = 0;
            double v = 0;
            rgb_to_hsv(r, g, b, h, s, v);
            // Rotate the hue values of the hsv image by the this amount
            h += shift;
            hsv_to_rgb(h, s, v, r, g, b);
            shifted[i] = r * 255;
            shifted[i + 1] = g * 255;
            shifted[i + 2] = b * 255;
        }
    }
}
