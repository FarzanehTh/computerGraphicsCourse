#include "hsv_to_rgb.h"
#include <cmath> 
// #include <iostream>

void hsv_to_rgb(
  const double h,
  const double s,
  const double v,
  double & r,
  double & g,
  double & b)
{
    // To convert HSV to RGB as follows, I used:
    // https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB

    // Find the chroma
    double chroma = s * v;
    // Adjust the hPrime to be between 0-360 by taking its fmod.
    double hPrime = fmod(h / 60.0, 6);

    // Then we can, again, find a point (R1, G1, B1) along the bottom three faces
    // of the RGB cube, with the same hue and chroma as our color (using the
    // intermediate value x for the second largest component of this color)
    double x = chroma * (1 - fabs(fmod(hPrime, 2) - 1));

    if (hPrime >= 0 && hPrime < 1) {
        r = chroma;
        g = x;
        b = 0;
    } else if (hPrime >= 1 && hPrime < 2) {
        r = x;
        g =chroma;
        b = 0;
    } else if (hPrime >= 2 && hPrime < 3) {
        r = 0;
        g = chroma;
        b = x;
    } else if (hPrime >= 3 && hPrime < 4) {
        r = 0;
        g = x;
        b = chroma;
    } else if (hPrime >= 4 && hPrime < 5) {
        r = x;
        g = 0;
        b = chroma;
    } else if (hPrime >= 5 && hPrime < 6) {
        r = chroma;
        g = 0;
        b = x;
    } else {
        // hPime >= 6, so we assign the colour 0 to rgb values
        r = 0;
        g = 0;
        b = 0;
    }

    // min rgb value is max of them minus range of them
    double min = v - chroma;
    // Increase each rgb value by the min value
    r += min;
    g += min;
    b += min;
}
