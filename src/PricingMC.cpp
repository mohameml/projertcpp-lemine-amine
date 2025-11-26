#include "PricingMC.hpp"
#include <numeric>      // for std::accumulate
#include <stdexcept>

PricingMC::PricingMC(const Option &opt, const Model &mod, int paths, int steps, double spot)
    : option_(opt) , model_(mod) , nPaths(paths) , nSteps(steps) , S0(spot)
{
    if (nPaths <= 0 || nSteps <= 0) {
        throw std::invalid_argument("Number of paths and steps must be positive");
    }
    
}

double PricingMC::price(double& price , double& demi_ic , double t , std::vector<double>& past) const
{
    // double price = 0.; 
    // double demi_ic = 0.;
    price = 0.; 
    demi_ic = 0.;

    std::vector<double> path;
    for (int i = 0; i < nPaths; ++i) {
        model_.generatePath(path, S0, option_.T, nSteps);
        double payoff = option_.payoff(path);
        double discounted = payoff * model_.discount(option_.T);

        price += discounted;
        demi_ic += std::pow(discounted , 2);
    }

    price /=  nPaths;
    demi_ic = std::sqrt((demi_ic - std::pow(price , 2)) / nPaths);
    demi_ic *= 1.96 / std::sqrt(nPaths);



}



double PricingMC::delta(double t , std::vector<double>& past , double h)  {
    double S_up = S0 * (1.0 + h); // S_0 + h 
    double S_down = S0 * (1.0 - h);

    // price_up : 
    this->S0 = S_up;
    double price_up;
    double demi_ic;
    price(price_up , demi_ic ,t , past );

    // price_down : 
    this->S0 = S_down;
    double price_down;
    price(price_down , demi_ic , t , past);

    return (price_up - price_down) / (S_up - S_down);

}
