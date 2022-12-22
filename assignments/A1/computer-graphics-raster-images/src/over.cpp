#include "over.h"

void over(
  const std::vector<unsigned char> & A,
  const std::vector<unsigned char> & B,
  const int & width,
  const int & height,
  std::vector<unsigned char> & C)
{
  C.resize(A.size());

  // The formual used in this function is from http://ssp.impulsetrain.com/porterduff.html
  
  // C = A over B such that "over" is the Porter-Duff "over" operator
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
        int i = (row * width + col) * 4;
        double alphaA = A[i + 3] / 255.0;
        double alphaB = B[i + 3] / 255.0;

        double areaA = alphaA * (1 - alphaB);
        double areaB = alphaB * (1 - alphaA);
        double areaBoth = alphaA * alphaB;

        // Since in the "Over" operation, we have the effect of both images A, B, and both, 
        // all of thier coefficients are 1. So we just add them all up.
        double resultantAlpha = areaA + areaB + areaBoth;

        C[i] = (unsigned char)((areaA * (double)A[i] + areaB * (double)B[i] + areaBoth * (double)A[i]));
        C[i + 1] = (unsigned char)((areaA * (double)A[i + 1] + areaB * (double)B[i + 1] + areaBoth * (double)A[i + 1]));
        C[i + 2] = (unsigned char)((areaA * (double)A[i + 2] + areaB * (double)B[i + 2] + areaBoth * (double)A[i + 2]));

        // I could also have the alphaA and alphaB as values greater than 1 and then
        // when filling in C[i], C[i + 1] and C[i + 2], I divide their values by resultantAlpha.
        C[i + 3] = (unsigned char)(resultantAlpha * 255);
    }
  }
}
