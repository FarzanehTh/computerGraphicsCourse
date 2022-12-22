// Given a 3d position as a seed, compute a smooth procedural noise
// value: "Perlin Noise", also known as "Gradient noise".
//
// Inputs:
//   st  3D seed
// Returns a smooth value between (-1,1)
//
// expects: random_direction, smooth_step
float perlin_noise( vec3 st) 
{
    // Implementing Perlin noise using https://en.wikipedia.org/wiki/Perlin_noise#Algorithm_detail
    float x = st[0];
    float y = st[1];
    float z = st[2];

    // 1. Determine the grid cell coordinates for this random 3-D seed
    int x0 = int(floor(x));
    int x1 = x0 + 1;
    int y0 = int(floor(y));
    int y1 = y0 + 1;
    int z0 = int(floor(z));
    int z1 = z0 + 1;

    // Now we have to calculate the Perlin noise for a cube with boundaries
    // (x0, y0) and (x1, y1). 
    // 2. Calculate the random gradient vectors for all 8 corners of the cube
    vec3 corners[8];
    corners[0] = vec3(x0, y0, z0);
    corners[1] = vec3(x1, y0, z0);
    corners[2] = vec3(x1, y1, z0);
    corners[3] = vec3(x0, y1, z0);
    corners[4] = vec3(x0, y0, z1);
    corners[5] = vec3(x1, y0, z1);
    corners[6] = vec3(x1, y1, z1);
    corners[7] = vec3(x0, y1, z1);

    // 3. Calculate random gradients for each corner
    vec3 gradients[8];
    for (int i = 0; i < 8; i++) {
        gradients[i] = random_direction(corners[i]);
    }


    // 4. Compute the distance of all 8 corners to the random point st
    vec3 distances[8];
    for (int i = 0; i < 8; i++) {
        distances[i] =  st - corners[i];
    }

    // 5. Compute the dot product of gradients with its corresponding distance
    float dotProducts[8];
    for (int i = 0; i < 8; i++) {
        dotProducts[i] = dot(gradients[i], distances[i]);
    }

    // 6. Compute the weight of the random point st and the corner (x0, y0, z0),
    // these are the interpolation weights.
    float sx = smooth_step(x - float(x0));
    float sy = smooth_step(y - float(y0));
    float sz = smooth_step(z - float(z0));

    // 7. Interpolate between the dotProducts computed for 
    // corners based on the formual (a1 - a0) * w + a0

    // Interpolate along the x
    // There will be 4 interpolations along the x
    float interpolationAlongX[4];
    for (int i = 0; i < 7; i += 2) {
        float a0 = dotProducts[i];
        float a1 = dotProducts[i + 1];
        // sx is like a weight for interpolation
        interpolationAlongX[i / 2] = (a1 - a0) * sx + a0;
    }

    // Interpolate along the y
    // There will be 2 interpolations along the x
    float interpolationAlongY[2];
    for (int i = 0; i < 4; i += 2) {
        float a0 = interpolationAlongX[i];
        float a1 = interpolationAlongX[i + 1];
        // sy is like a weight for interpolation
        interpolationAlongY[i / 2] = (a1 - a0) * sy + a0;
    }

    // Interpolate along the z
    float a0 = interpolationAlongY[0];
    float a1 = interpolationAlongY[1];
    // sz is like a weight for interpolation
    return ((a1 - a0) * sz + a0);
}

