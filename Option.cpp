#include "Option.hpp"
#include <algorithm>

// =================== CallVanillaOption : ================
CallVanillaOption::CallVanillaOption(double strike, double maturity)
    : Option(maturity), K_(strike) {}

double CallVanillaOption::payoff(const std::vector<double>& path) const {
    // TODO ...
}


// ==================== PutVanillaOption : =================
PutVanillaOption::PutVanillaOption(double strike, double maturity)
    : Option(maturity), K_(strike) {}

double PutVanillaOption::payoff(const std::vector<double>& path) const {
    // TODO ....
}


// ==================== LookBackCallOption : =================
LookBackCallOption::LookBackCallOption(double maturity)
    : Option(maturity) {}

double LookBackCallOption::payoff(const std::vector<double>& path) const {
    // TODO ...
}


// ==================== LookBackPutOption : =================
LookBackPutOption::LookBackPutOption(double maturity)
    : Option(maturity) {}

double LookBackPutOption::payoff(const std::vector<double>& path) const {
    // TODO ...
}