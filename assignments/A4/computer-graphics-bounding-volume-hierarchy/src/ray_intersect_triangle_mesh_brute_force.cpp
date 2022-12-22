#include "ray_intersect_triangle_mesh_brute_force.h"
#include "ray_intersect_triangle.h"

bool ray_intersect_triangle_mesh_brute_force(
  const Ray & ray,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double min_t,
  const double max_t,
  double & hit_t,
  int & hit_f)
{
    double temp_hit_t;
    hit_t = std::numeric_limits<double>::infinity();
    bool rayHit = false;

    // Each row of the faces list F refers to one trianlge
    for (int i = 0; i < F.rows(); i++) {
        Eigen::RowVector3i indeciesIntoVertices = F.row(i);
        Eigen::RowVector3d A = V.row(indeciesIntoVertices(0));
        Eigen::RowVector3d B = V.row(indeciesIntoVertices(1));
        Eigen::RowVector3d C = V.row(indeciesIntoVertices(2));
        rayHit = ray_intersect_triangle(ray, A, B, C, min_t, max_t, temp_hit_t);
        if (rayHit) {
            if (hit_t > temp_hit_t) {
                hit_t = temp_hit_t;
                hit_f = i;
            }
        }
    }
    return rayHit;
}
