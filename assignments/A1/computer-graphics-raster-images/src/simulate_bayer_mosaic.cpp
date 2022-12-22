#include "simulate_bayer_mosaic.h"

void simulate_bayer_mosaic(
  const std::vector<unsigned char> & rgb,
  const int & width,
  const int & height,
  std::vector<unsigned char> & bayer)
{
    bayer.resize(width*height);

    // This is simulating the reverse of the Bayers colour filtering.
    // i.e. assigns each pixel to only contain the colour that it would
    // have got through the Bayers colour filter
    for (int row = 0; row < height; row += 2) {
        for (int col = 0; col < width; col += 2) {
            int i = row * width + col;
            // Take Green
            bayer[i] = rgb[i * 3 + 1];
            // Take Blue
            bayer[i + 1] = rgb[i * 3 + 2];
            
            int t = (row + 1) * width + col;
            if (t < width * height) {
                // Take Red
                bayer[t] = rgb[t * 3];
                // Take Green
                bayer[t + 1] = rgb[t * 3 + 1];
            }
        }
    }
}
