#include "Model.hpp"
#include <cmath>
#include <stdexcept>
#include  <algorithm>

// -------------------- BSModel --------------------
BSModel::BSModel(double r, double sigma, unsigned long seed)
    : r_(r), sigma_(sigma), rng_(seed), nd_(0.0, 1.0)
{
   // TODO ....
}

void BSModel::generatePath(std::vector<double>& path,
                           double S0,
                           double T,
                           int nSteps) const
{
    // TODO ....
}