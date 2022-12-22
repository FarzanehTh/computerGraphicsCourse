#include "desaturate.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void desaturate(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double factor,
  std::vector<unsigned char> & desaturated)
{
    desaturated.resize(rgb.size());

    // The factor fractional amount of saturation to remove:
    // 1 -- > fully grayscale, 0-->retain input color.

    // Turn the rgb image to hsv image
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int i = (row * width + col) * 3;
            double r = (double)rgb[i] / 255.0;
            double g = (double)rgb[i + 1] / 255.0;
            double b = (double)rgb[i + 2] / 255.0;
            double h = 0;
            double s = 0;
            double v = 0;
            rgb_to_hsv(r, g, b, h, s, v);
            // desaturate by 1 - factor
            s *= (double)(1 - factor);
            hsv_to_rgb(h, s, v, r, g, b);
            desaturated[i] = (unsigned char)(r * 255.0);
            desaturated[i + 1] = (unsigned char)(g * 255.0);
            desaturated[i + 2] = (unsigned char)(b * 255.0);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
}
