#ifndef _MODEL_
#define _MODEL_

#include <vector>
#include <random>
#include <cmath>
#include <functional>

// ========= Abstract Class Model : =============
class Model {
public:
    virtual ~Model() = default;

    // génère un path de taille nSteps+1 dans path (path doit être dimensionné)
    virtual void generatePath(std::vector<double>& path,
                              double S0,
                              double T,
                              int nSteps) const = 0;

    // discount factor e^{-r T}
    virtual double discount(double T) const = 0;
};

// ============== Class Model : =============

class BSModel : public Model {
private:
    double r_;
    double sigma_;
    mutable std::mt19937_64 rng_;
    mutable std::normal_distribution<double> nd_;

public:
    BSModel(double r, double sigma, unsigned long seed = 42);

    double r() const { return r_; }
    double sigma() const { return sigma_; }

    void generatePath(std::vector<double>& path,
                      double S0,
                      double T,
                      int nSteps) const override;

    double discount(double T) const override {
        return std::exp(-r_ * T);
    }
};

#endif

class BinomialModel : public Model {
private:
    double r_;
    double sigma_;
    int nSteps_;
    mutable std::mt19937_64 rng_;  // RNG member

public:
    BinomialModel(double r, double sigma, int nSteps);

    void generatePath(std::vector<double>& path, double S0, double T, int unused) const override;

    double discount(double T) const override { return std::exp(-r_ * T); }

    // Additional methods for tree construction and option pricing via backward induction
    std::vector<std::vector<double>> buildPriceTree(double S0, double T) const;

    // (You may want a specialized method for option pricing rather than path generation)
};

#endif

class LSVModel : public Model {
private:
    double r_;
    double kappa_;
    double theta_;
    double xi_;
    double rho_;

    mutable std::mt19937_64 rng_;
    mutable std::normal_distribution<double> nd_;

    std::function<double(double,double)> sigmaLocal_;  // local vol function sigma_loc(S,t)

public:
    LSVModel(double r, double kappa, double theta, double xi, double rho,
             std::function<double(double,double)> sigmaLocal,
             unsigned long seed);

    void generatePath(std::vector<double>& path, double S0, double T, int nSteps) const override;

    double discount(double T) const override { return std::exp(-r_ * T); }
};
#endif

class HestonModel : public Model {
private:
    double r_;
    double kappa_;
    double theta_;
    double xi_;
    double rho_;

    mutable std::mt19937_64 rng_;
    mutable std::normal_distribution<double> nd_;

public:
    HestonModel(double r, double kappa, double theta, double xi, double rho,
                unsigned long seed);

    void generatePath(std::vector<double>& path, double S0, double T, int nSteps) const override;

    double discount(double T) const override { return std::exp(-r_ * T); }

    void HestonModel::generateAssetAndVariancePaths(std::vector<double>& assetPath,
                                               std::vector<double>& variancePath,
                                               double S0,
                                               double T,
                                               int nSteps) const
{
    assetPath.resize(nSteps + 1);
    variancePath.resize(nSteps + 1);

    assetPath[0] = S0;
    variancePath[0] = theta_;

    double dt = T / nSteps;
    double v = theta_;

    for (int i = 1; i <= nSteps; ++i) {
        double Z1 = nd_(rng_);
        double Z2 = nd_(rng_);
        double W1 = Z1;
        double W2 = rho_ * Z1 + std::sqrt(1.0 - rho_ * rho_) * Z2;

        v = std::max(v + kappa_ * (theta_ - v) * dt + xi_ * std::sqrt(std::max(v, 0.0)) * std::sqrt(dt) * W2, 0.0);
        variancePath[i] = v;

        double drift = (r_ - 0.5 * v) * dt;
        double diffusion = std::sqrt(v * dt) * W1;
        assetPath[i] = assetPath[i-1] * std::exp(drift + diffusion);
    }
}
};

#endif