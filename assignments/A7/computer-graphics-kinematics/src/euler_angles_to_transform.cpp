#include "euler_angles_to_transform.h"
#include <Eigen/Geometry>


Eigen::Affine3d euler_angles_to_transform(
  const Eigen::Vector3d & xzx)
{
    // Change the angle from degree to radian
    double theta1 = xzx[0] * (M_PI / 180);
    double theta2 = xzx[1] * (M_PI / 180);
    double theta3 = xzx[2] * (M_PI / 180);

    Eigen::Matrix4d rotationAboutX1;
    rotationAboutX1 << 
        1, 0, 0, 0,
        0, cos(theta1), -sin(theta1), 0,
        0, sin(theta1), cos(theta1), 0,
        0, 0, 0, 1;

    Eigen::Matrix4d rotationAboutZ;
    rotationAboutZ << 
        cos(theta2), -sin(theta2), 0, 0,
        sin(theta2), cos(theta2), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    Eigen::Matrix4d rotationAboutX2;
    rotationAboutX2 << 
        1, 0, 0, 0,
        0, cos(theta3), -sin(theta3), 0,
        0, sin(theta3), cos(theta3), 0,
        0, 0, 0, 1;

    Eigen::Affine3d A;
    A.matrix() = rotationAboutX2 * rotationAboutZ * rotationAboutX1;
    return A;
}
