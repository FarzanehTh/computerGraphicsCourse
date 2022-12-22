#include "triangle_triangle_intersection.h"
#include "Ray.h"
#include "ray_intersect_triangle.h"
#include <vector>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <iostream>

bool triangle_triangle_intersection(
  const Eigen::RowVector3d & A0,
  const Eigen::RowVector3d & A1,
  const Eigen::RowVector3d & A2,
  const Eigen::RowVector3d & B0,
  const Eigen::RowVector3d & B1,
  const Eigen::RowVector3d & B2)
{
    // Consider the edges of the triangle B as the rays hitting the triangle A,
    // then see if each ray hits the triangle A. Do the same for both.

    Ray rayB0B1(B0.transpose(), (B1 - B0).transpose());
    double t1;
    double rayB0B1_min_t = 0;
    double rayB0B1_max_t = 1;
    if (ray_intersect_triangle(rayB0B1, A0, A1, A2, rayB0B1_min_t, rayB0B1_max_t, t1)) {
        return true;
    }

    Ray rayB0B2(B0.transpose(), (B2 - B0).transpose());
    double t2;
    double rayB0B2_min_t = 0;
    double rayB0B2_max_t = 1;
    if (ray_intersect_triangle(rayB0B2, A0, A1, A2, rayB0B2_min_t, rayB0B2_max_t, t2)) {
        return true;
    }

    Ray rayB1B2(B1.transpose(), (B2 - B1).transpose());
    double t3;
    double rayB1B2_min_t = 0;
    double rayB1B2_max_t = 1;
    if (ray_intersect_triangle(rayB1B2, A0, A1, A2, rayB1B2_min_t, rayB1B2_max_t, t3)) {
        return true;
    }

    Ray rayA0A1(A0.transpose(), (A1 - A0).transpose());
    double t4;
    double rayA0A1_min_t = 0;
    double rayA0A1_max_t = 1;
    if (ray_intersect_triangle(rayA0A1, B0, B1, B2, rayA0A1_min_t, rayA0A1_max_t, t4)) {
        return true;
    }

    Ray rayA0A2(A0.transpose(), (A2 - A0).transpose());
    double t5;
    double rayA0A2_min_t = 0;
    double rayA0A2_max_t = 1;
    if (ray_intersect_triangle(rayA0A2, B0, B1, B2, rayA0A2_min_t, rayA0A2_max_t, t5)) {
        return true;
    }

    Ray rayA1A2(A1.transpose(), (A2 - A1).transpose());
    double t6;
    double rayA1A2_min_t = 0;
    double rayA1A2_max_t = 1;
    if (ray_intersect_triangle(rayA1A2, B0, B1, B2, rayA1A2_min_t, rayA1A2_max_t, t6)) {
        return true;
    }

    return false;
}
