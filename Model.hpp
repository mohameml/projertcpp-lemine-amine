#ifndef _MODEL_
#define _MODEL_

#include <vector>
#include <random>

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