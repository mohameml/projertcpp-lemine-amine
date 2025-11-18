#ifndef _OPTION_
#define _OPTION_
#include <vector>


// ============ Abstract class for Option : ================
class Option {
public:
    double T; // maturité en années
    Option(double maturity = 0.0) : T(maturity) {}
    virtual ~Option() = default;

    // path : vecteur S_0, S_1, ..., S_n
    virtual double payoff(const std::vector<double>& path) const = 0;
};

// Vanilla Call
class CallVanillaOption : public Option {
private:
    double K_;
public:
    CallVanillaOption(double strike, double maturity);
    double payoff(const std::vector<double>& path) const override;
};

// Vanilla Put
class PutVanillaOption : public Option {
private:
    double K_;
public:
    PutVanillaOption(double strike, double maturity);
    double payoff(const std::vector<double>& path) const override;
};

// Lookback Call (floating strike: S_T - min_{t} S_t)
class LookBackCallOption : public Option {
public:
    LookBackCallOption(double maturity);
    double payoff(const std::vector<double>& path) const override;
};

// Lookback Put (floating strike: max_{t} S_t - S_T)
class LookBackPutOption : public Option {
public:
    LookBackPutOption(double maturity);
    double payoff(const std::vector<double>& path) const override;
};

#endif 