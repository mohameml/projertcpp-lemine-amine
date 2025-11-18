#ifndef _PRICING_MC_
#define _PRICING_MC_

#include "Option.hpp"
#include "Model.hpp"

class PricingMC {
private:
    const Option& option_;
    const Model& model_;

public:
    int nPaths;
    int nSteps;
    double S0;

    PricingMC(const Option& opt,
              const Model& mod,
              int paths = 10000,
              int steps = 252,
              double spot = 100.0);

    // prix Monte-Carlo standard (estimation simple, non vectorisée)
    double price() const;
};

#endif 