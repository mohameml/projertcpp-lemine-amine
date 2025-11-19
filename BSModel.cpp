#include "Model.hpp"
#include <cmath>
#include <stdexcept>
#include <algorithm>

// -------------------- BSModel --------------------

BSModel::BSModel(double r, double sigma, unsigned long seed)
    : r_(r), sigma_(sigma), rng_(seed), nd_(0.0, 1.0)
{
    if (sigma < 0.0) {
        throw std::invalid_argument("Volatility sigma must be non-negative");
    }
    if (r < 0.0) {
        // Not always invalid, but often rates are positive; you can relax if needed
        // throw std::invalid_argument("Risk-free rate r must be non-negative");
    }
}

void BSModel::generatePath(std::vector<double>& path,
                           double S0,
                           double T,
                           int nSteps) const
{
    if (nSteps <= 0) {
        throw std::invalid_argument("nSteps must be positive");
    }
    if (S0 <= 0.0) {
        throw std::invalid_argument("Initial price S0 must be positive");
    }
    path.resize(nSteps + 1);
    path[0] = S0;

    double dt = T / nSteps;
    double drift = (r_ - 0.5 * sigma_ * sigma_) * dt;
    double diffusion_coefficient = sigma_ * std::sqrt(dt);

    for (int i = 1; i <= nSteps; ++i) {
        double Z = nd_(rng_);
        path[i] = path[i-1] * std::exp(drift + diffusion_coefficient * Z);
    }
}

// -------------------- HESTON Model --------------------
// Constructor
HestonModel::HestonModel(double r, double kappa, double theta, double xi, double rho, unsigned long seed)
    : r_(r), kappa_(kappa), theta_(theta), xi_(xi), rho_(rho), rng_(seed), nd_(0.0, 1.0)
{
    if (kappa < 0.0)
        throw std::invalid_argument("Mean reversion kappa must be non-negative");
    if (theta < 0.0)
        throw std::invalid_argument("Long-term variance theta must be non-negative");
    if (xi < 0.0)
        throw std::invalid_argument("Volatility of variance xi must be non-negative");
    if (rho < -1.0 || rho > 1.0)
        throw std::invalid_argument("Correlation rho must be in [-1,1]");
}

// Generate path for underlying price S_t, ignoring variance path return (could be extended)
void HestonModel::generatePath(std::vector<double>& path,
                              double S0,
                              double T,
                              int nSteps) const
{
    if (nSteps <= 0)
        throw std::invalid_argument("nSteps must be positive");
    if (S0 <= 0.0)
        throw std::invalid_argument("Initial price S0 must be positive");

    path.resize(nSteps + 1);
    path[0] = S0;

    double dt = T / nSteps;
    double v = theta_;  // start variance at long-term mean

    for (int i = 1; i <= nSteps; ++i) {
        // Generate two correlated standard normals using Cholesky decomposition
        double Z1 = nd_(rng_);
        double Z2 = nd_(rng_);
        double W1 = Z1;
        double W2 = rho_ * Z1 + std::sqrt(1.0 - rho_ * rho_) * Z2;

        // Variance process using Euler discretization (Cox-Ingersoll-Ross)
        v = std::max(v + kappa_ * (theta_ - v) * dt + xi_ * std::sqrt(std::max(v, 0.0)) * std::sqrt(dt) * W2, 0.0);

        // Underlying price process
        double drift = (r_ - 0.5 * v) * dt;
        double diffusion = std::sqrt(v * dt) * W1;
        path[i] = path[i-1] * std::exp(drift + diffusion);
    }
}

// Discount factor - same as BS model
double HestonModel::discount(double T) const
{
    return std::exp(-r_ * T);
}


// -------------------- LSV Model --------------------
// Constructor
LSVModel::LSVModel(double r, double kappa, double theta, double xi, double rho,
                   std::function<double(double,double)> sigmaLocal,
                   unsigned long seed)
    : r_(r), kappa_(kappa), theta_(theta), xi_(xi), rho_(rho),
      sigmaLocal_(sigmaLocal), rng_(seed), nd_(0.0, 1.0)
{
    if (kappa < 0.0)
        throw std::invalid_argument("Mean reversion kappa must be non-negative");
    if (theta < 0.0)
        throw std::invalid_argument("Long-term variance theta must be non-negative");
    if (xi < 0.0)
        throw std::invalid_argument("Volatility xi must be non-negative");
    if (rho < -1.0 || rho > 1.0)
        throw std::invalid_argument("Correlation rho must be in [-1,1]");
}

// Generate path with local volatility modulating stochastic volatility process
void LSVModel::generatePath(std::vector<double>& path,
                            double S0,
                            double T,
                            int nSteps) const
{
    if (nSteps <= 0)
        throw std::invalid_argument("nSteps must be positive");
    if (S0 <= 0.0)
        throw std::invalid_argument("Initial price S0 must be positive");

    path.resize(nSteps + 1);
    path[0] = S0;

    double dt = T / nSteps;
    double v = theta_;  // start variance at long-term mean

    double t = 0.0;

    for (int i = 1; i <= nSteps; ++i) {
        t += dt;

        // Generate correlated normals
        double Z1 = nd_(rng_);
        double Z2 = nd_(rng_);
        double W1 = Z1;
        double W2 = rho_ * Z1 + std::sqrt(1.0 - rho_ * rho_) * Z2;

        // Update variance (CIR with Euler discretization)
        v = std::max(v + kappa_ * (theta_ - v) * dt + xi_ * std::sqrt(std::max(v, 0.0)) * std::sqrt(dt) * W2, 0.0);

        // Local vol factor at current price and time
        double sigma_loc = sigmaLocal_(path[i-1], t);

        // Underlying price update with local stochastic vol
        double drift = (r_ - 0.5 * v * sigma_loc * sigma_loc) * dt;
        double diffusion = sigma_loc * std::sqrt(v * dt) * W1;

        path[i] = path[i-1] * std::exp(drift + diffusion);
    }
}

double LSVModel::discount(double T) const
{
    return std::exp(-r_ * T);
}



// -------------------- BINOMIAL Model --------------------
// Constructor

BinomialModel::BinomialModel(double r, double sigma, int nSteps)
    : r_(r), sigma_(sigma), nSteps_(nSteps)
{
    if (sigma < 0.0)
        throw std::invalid_argument("Volatility sigma must be non-negative");
    if (nSteps <= 0)
        throw std::invalid_argument("Number of steps must be positive");
}

void BinomialModel::generatePath(std::vector<double>& path,
                                double S0,
                                double T,
                                int unused) const
{
    // Note: generating a single path in a binomial model is less standard.
    // Here, generate one possible upward/downward path randomly.

    if (S0 <= 0.0)
        throw std::invalid_argument("Initial price S0 must be positive");

    path.resize(nSteps_ + 1);
    path[0] = S0;

    double dt = T / nSteps_;
    double u = std::exp(sigma_ * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp(r_ * dt) - d) / (u - d);

    // Simple RNG for Bernoulli outcome
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    for (int i = 1; i <= nSteps_; ++i) {
        double randVal = uniform(rng);
        if (randVal < p) {
            path[i] = path[i-1] * u;  // up move
        } else {
            path[i] = path[i-1] * d;  // down move
        }
    }
}

double BinomialModel::discount(double T) const
{
    return std::exp(-r_ * T);
}
