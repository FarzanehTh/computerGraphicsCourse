#include "rgb_to_hsv.h"
#include <algorithm>
#include <cmath>

void rgb_to_hsv(
  const double r,
  const double g,
  const double b,
  double & h,
  double & s,
  double & v)
{
    // To convert RGB to HSV as follows, I used this:
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Hue_and_chroma

    double max = std::max({r, g, b});
    double min = std::min({r, g, b});
    // Value
    v = max;
    
    double chroma = max - min;
    
    // Saturation
    if (v == 0) {
        s = 0;
    } else {
        s = chroma / v;
    }

    // Hue
    if (chroma < 0.000001) {
        // If the chroma is 0 or very close to 0, the hue is undefined. 
        // We can also assign it any other value, like 0, since it will 
        // have the same effect.
        h = 0;
    } else if (max == r) {
        h = fmod((double)((g - b) / chroma), 6);
    } else if (max == g) {
        h = ((b - r) / chroma) + 2.0;
    } else if(max == b) {
        h = ((r - g) / chroma) + 4.0;
    }
    h = h * 60.0;
}
