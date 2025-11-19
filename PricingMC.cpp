#include "PricingMC.hpp"
#include <numeric>      // for std::accumulate
#include <stdexcept>

double PricingMC::price() const {
    if (nPaths <= 0 || nSteps <= 0) {
        throw std::invalid_argument("Number of paths and steps must be positive");
    }

    // Storage for all discounted payoffs
    std::vector<double> discountedPayoffs;
    discountedPayoffs.reserve(nPaths);

    for (int i = 0; i < nPaths; ++i) {
        std::vector<double> path;
        model_.generatePath(path, S0, option_.T, nSteps);

        // Compute payoff for this path
        double payoff = option_.payoff(path);

        // Discount the payoff to present value
        double discounted = payoff * model_.discount(option_.T);

        discountedPayoffs.push_back(discounted);
    }

    // Compute Monte Carlo estimate (average)
    double sumPayoffs = std::accumulate(discountedPayoffs.begin(), discountedPayoffs.end(), 0.0);

    return sumPayoffs / nPaths;
}
