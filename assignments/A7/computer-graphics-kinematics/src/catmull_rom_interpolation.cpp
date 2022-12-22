#include "catmull_rom_interpolation.h"
#include <Eigen/Dense>
#include <iostream>

Eigen::Vector3d catmull_rom_interpolation(
  const std::vector<std::pair<double, Eigen::Vector3d> > & keyframes,
  double t)
{
    Eigen::Vector3d interpolatedPoint(0, 0, 0);
    if (keyframes.size() == 0) {
        return interpolatedPoint;
    }

    // Make sure the time t is within the ranges defined by the keyframes,
    // if bigger warp it to be within the limits by the mod operation.
    // Note that the keyframes are sorted based on time, so the last
    // one refers to the max value of time.
    double time = std::fmod(t, keyframes.back().first);

    Eigen::Vector3d theta0;
    Eigen::Vector3d theta1;
    Eigen::Vector3d theta2;
    Eigen::Vector3d theta3;
    double t0;
    double t1;
    double t2;
    double t3;
    std::vector<std::pair<double, Eigen::Vector3d>> greaterPoints;
    std::vector<std::pair<double, Eigen::Vector3d>> smallerPoints;
    for (std::pair<double, Eigen::Vector3d> pair: keyframes) {
        if (time < pair.first) {
            greaterPoints.push_back(pair);
        } else {
            smallerPoints.push_back(pair);
        }
    }
    // It is guaranteed that the time "time" will be between two values,
    // so assign those two values as t1 and t1.
    t1 = (smallerPoints.back()).first;
    t2 = (greaterPoints.front()).first;
    theta1 = (smallerPoints.back()).second;
    theta2 = (greaterPoints.front()).second;

    // Now remove the last element in the smaller values and assign
    // it as t0 if there remains any elements. If not, re-use t1 for it.
    smallerPoints.pop_back();
    if (smallerPoints.size() > 0) {
        t0 = smallerPoints.back().first;
        theta0 = smallerPoints.back().second;
        t0 = t1;
        theta0 = theta1;
    } else {
        t0 = t1;
        theta0 = theta1;
    }

    // Now remove the first element in the smaller values and assign
    // it as t3 if there remains any elements. If not, re-use t2 for it.
    greaterPoints.erase(greaterPoints.begin());
    if (greaterPoints.size() > 0) {
        t3 = (greaterPoints.front()).first;
        theta3 = (greaterPoints.front()).second;
    } else {
        t3 = t2;
        theta3 = theta2;
    }

    // Make sure the time is between 0 and 1
    double tUnit = (time - t1) / (t2 - t1);

    Eigen::Vector3d m1 = (theta2 - theta0) / (t2 - t0);
    Eigen::Vector3d m2 = (theta3 - theta1) / (t3 - t1);

    interpolatedPoint = (2 * pow(tUnit, 3) - 3 * pow(tUnit, 2) + 1) * theta1 +
        (pow(tUnit, 3) - 2 * pow(tUnit, 2) + tUnit) * m1 +
        (-2 * pow(tUnit, 3) + 3 * pow(tUnit, 2)) * theta2 + 
        (pow(tUnit, 3) - pow(tUnit, 2)) * m2;

    return interpolatedPoint;
}
