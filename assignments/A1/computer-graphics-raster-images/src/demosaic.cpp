#include "demosaic.h"

int computeCornerNeighboursAverage(const std::vector<unsigned char>& bayer, const int& row,
    const int& col, const int& width, const int& height) {
        
    int topLeft = (row - 1) * width + (col - 1);
    int topRight = (row - 1) * width + (col + 1);
    int buttomLeft = (row + 1) * width + (col - 1);
    int buttomRight = (row + 1) * width + (col + 1);

    topLeft = (topLeft >= 0 && topLeft < width * height) ? topLeft : 0;
    topLeft = (topLeft >= 0 && topLeft < width * height) ? topLeft : 0;

    int sum = 0;
    int count = 0;
    if (topLeft >= 0 && topLeft < width * height) {
        sum += bayer[topLeft];
        count++;
    }
    if (topRight >= 0 && topRight < width * height) {
        sum += bayer[topRight];
        count++;
    }
    if (buttomLeft >= 0 && buttomLeft < width * height) {
        sum += bayer[buttomLeft];
        count++;
    }
    if (buttomRight >= 0 && buttomRight < width * height) {
        sum += bayer[buttomRight];
        count++;
    }
    return sum / count;
}

int getGreenNeighboursAverage(const std::vector<unsigned char>& bayer, const int& row,
    const int& col, const int& width, const int& height) {

    int top = (row - 1) * width + col;
    int buttom = (row + 1) * width + col;
    int left = row * width + (col - 1);
    int right = row * width + (col + 1);

    int sumGreen = 0;
    int countGreen = 0;

    if (top >= 0 && top < width * height) {
        sumGreen += bayer[top];
        countGreen++;
    }
    if (buttom >= 0 && buttom < width * height) {
        sumGreen += bayer[buttom];
        countGreen++;
    }
    if (left >= 0 && left < width * height) {
        sumGreen += bayer[left];
        countGreen++;
    }
    if (right >= 0 && right < width * height) {
        sumGreen += bayer[right];
        countGreen++;
    }
    return sumGreen / countGreen;
}

void demosaic(const std::vector<unsigned char>& bayer, const int& width, const int& height, 
    std::vector<unsigned char>& rgb) {

    rgb.resize(width * height * 3);

    // Turn the mosaicized image (using Bayers filter) to a colour image.
    // i.e. Demosaicize the image
    // To demosaicize, we need to take an average of all 8 neighbouring cells

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int i = row * width + col;
            // Fill in the indexes i * 3, i * 3 + 1, i * 3 + 2 with values of RGB
            if (row % 2 == 0) {
                // Even rows will have Green, Blue, Green, Blue, ... pattern
                if (col % 2 == 0) {
                    // i is Green
                    // The top and buttom of index i are the red cells
                    int top = (row - 1) * width + col;
                    int buttom = (row + 1) * width + col;
                    rgb[i * 3] = (bayer[top] + bayer[buttom]) / 2;

                    // The index i has the green value
                    rgb[i * 3 + 1] = bayer[i];

                    // The left and right indexes of i are blue cells
                    int left = row * width + (col - 1);
                    int right = row * width + (col + 1);
                    rgb[i * 3 + 2] = (bayer[left] + bayer[right]) / 2;
                } else {
                    // i is Blue
                    // Compute the average Blue from the neighbours
                    rgb[i * 3] = computeCornerNeighboursAverage(bayer, row, col, width, height);

                    // Compute the average Green from the neighbours
                    rgb[i * 3 + 1] = getGreenNeighboursAverage(bayer, row, col, width, height);

                    // i is Blue
                    rgb[i * 3 + 2] = bayer[i];
                }

            } else {
                // Odd rows will have Red, Green, Red, Green, ... pattern
                if (col % 2 == 0) {
                    // i is Red
                    rgb[i * 3] = bayer[i];
                    rgb[i * 3 + 1] = getGreenNeighboursAverage(bayer, row, col, width, height);
                    rgb[i * 3 + 2] = computeCornerNeighboursAverage(bayer, row, col, width, height);

                } else {
                    // i is Green
                    // The left and right indexes of i are red cells
                    int left = row * width + (col - 1);
                    int right = row * width + (col + 1);
                    rgb[i * 3] = (bayer[left] + bayer[right]) / 2;

                    // The index i has the green value
                    rgb[i * 3 + 1] = bayer[i];

                    // The top and buttom of index i are the blue cells
                    int top = (row - 1) * width + col;
                    int buttom = (row + 1) * width + col;
                    rgb[i * 3 + 2] = (bayer[top] + bayer[buttom]) / 2;
                }
            }
        }
    }
}
