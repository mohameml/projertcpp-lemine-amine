#include "PricingMC.hpp"
#include <stdexcept>

PricingMC::PricingMC(const Option& opt,
                     const Model& mod,
                     int paths,
                     int steps,
                     double spot)
    : option_(opt), model_(mod), nPaths(paths), nSteps(steps), S0(spot)
{
    if (nPaths <= 0) throw std::invalid_argument("nPaths must be > 0");
    if (nSteps <= 0) throw std::invalid_argument("nSteps must be > 0");
}

double PricingMC::price() const {
    // TODO ... 
}