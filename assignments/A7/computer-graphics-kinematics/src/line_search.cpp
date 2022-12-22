#include "line_search.h"
#include <iostream>

double line_search(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const Eigen::VectorXd & z,
  const Eigen::VectorXd & dz,
  const double max_step)
{
    int iter = 0;
    int maxIteration = 100;
    double sigma = max_step;
    // z  #z vector of initial z values
    //   dz  #z vector of changes to z
    Eigen::VectorXd zResult = z + sigma * dz;
    proj_z(zResult);
    while (f(zResult) >= f(z) && iter < maxIteration) {
        sigma = sigma / 2;
        zResult = z + sigma * dz;
        proj_z(zResult);
        iter++;
    }
    return sigma;
}
