#ifndef _OPTION_
#define _OPTION_
#include <vector>
#include <algorithm> // for min_element, max_element
#include <stdexcept> // for exceptions


// ============ Abstract class for Option ================
class Option {
public:
    double T; // maturity in years

    Option(double maturity = 0.0) : T(maturity) {
        if (T <= 0.0)
            throw std::invalid_argument("Maturity must be positive");
    }

    virtual ~Option() = default;

    // path: vector of prices S_0, S_1, ..., S_n
    virtual double payoff(const std::vector<double>& path) const = 0;
};


// Vanilla Call
class CallVanillaOption : public Option {
private:
    double K_;

public:
    CallVanillaOption(double strike, double maturity)
        : Option(maturity), K_(strike) {
        if (K_ <= 0.0)
            throw std::invalid_argument("Strike must be positive");
    }

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Price path is empty");
        // European style payoff depends only on final price
        double S_T = path.back();
        return std::max(S_T - K_, 0.0);
    }
};


// Vanilla Put
class PutVanillaOption : public Option {
private:
    double K_;

public:
    PutVanillaOption(double strike, double maturity)
        : Option(maturity), K_(strike) {
        if (K_ <= 0.0)
            throw std::invalid_argument("Strike must be positive");
    }

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Price path is empty");
        double S_T = path.back();
        return std::max(K_ - S_T, 0.0);
    }
};


// Lookback Call (floating strike: S_T - min_{t} S_t)
class LookBackCallOption : public Option {
public:
    LookBackCallOption(double maturity) : Option(maturity) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Price path is empty");
        double S_T = path.back();
        double minPrice = *std::min_element(path.begin(), path.end());
        return std::max(S_T - minPrice, 0.0);
    }
};


// Lookback Put (floating strike: max_{t} S_t - S_T)
class LookBackPutOption : public Option {
public:
    LookBackPutOption(double maturity) : Option(maturity) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Price path is empty");
        double S_T = path.back();
        double maxPrice = *std::max_element(path.begin(), path.end());
        return std::max(maxPrice - S_T, 0.0);
    }
};

// ------ Digital Call --------
class DigitalCallOption : public Option {
private:
    double K_;
    double payout_;
public:
    DigitalCallOption(double strike, double maturity, double payout=1.0)
        : Option(maturity), K_(strike), payout_(payout) {}
    double payoff(const std::vector<double>& path) const override {
        if (path.empty()) throw std::invalid_argument("Path is empty");
        return (path.back() > K_) ? payout_ : 0.0;
    }
};

// ------ Digital Put --------
class DigitalPutOption : public Option {
private:
    double K_;
    double payout_;
public:
    DigitalPutOption(double strike, double maturity, double payout=1.0)
        : Option(maturity), K_(strike), payout_(payout) {}
    double payoff(const std::vector<double>& path) const override {
        if (path.empty()) throw std::invalid_argument("Path is empty");
        return (path.back() < K_) ? payout_ : 0.0;
    }
};


// ------ Asian Call --------
enum class AsianType { Arithmetic, Geometric };

class AsianCallOption : public Option {
private:
    double K_;
    AsianType type_;
public:
    AsianCallOption(double strike, double maturity, AsianType type=AsianType::Arithmetic)
        : Option(maturity), K_(strike), type_(type) {}
    double payoff(const std::vector<double>& path) const override {
        if (path.empty()) throw std::invalid_argument("Path is empty");
        double avg = 0.0;
        if (type_ == AsianType::Arithmetic) {
            avg = std::accumulate(path.begin(), path.end(), 0.0) / path.size();
        } else {
            double sumLog = 0.0;
            for (double p : path) sumLog += std::log(p);
            avg = std::exp(sumLog / path.size());
        }
        return std::max(avg - K_, 0.0);
    }
};

// ------ Asian Put --------
class AsianPutOption : public Option {
private:
    double K_;
    AsianType type_;
public:
    AsianPutOption(double strike, double maturity, AsianType type=AsianType::Arithmetic)
        : Option(maturity), K_(strike), type_(type) {}
    double payoff(const std::vector<double>& path) const override {
        if (path.empty()) throw std::invalid_argument("Path is empty");
        double avg = 0.0;
        if (type_ == AsianType::Arithmetic) {
            avg = std::accumulate(path.begin(), path.end(), 0.0) / path.size();
        } else {
            double sumLog = 0.0;
            for (double p : path) sumLog += std::log(p);
            avg = std::exp(sumLog / path.size());
        }
        return std::max(K_ - avg, 0.0);
    }
};


// ------ American Call Option -------
class AmericanCallOption : public Option {
private:
    double K_;
public:
    AmericanCallOption(double strike, double maturity)
        : Option(maturity), K_(strike) {}
    double payoff(const std::vector<double>& path) const override {
        if (path.empty()) throw std::invalid_argument("Path is empty");
        double maxPayoff = 0.0;
        for (double price : path) {
            double intrinsic = std::max(price - K_, 0.0);
            if (intrinsic > maxPayoff) maxPayoff = intrinsic;
        }
        return maxPayoff;
    }
};

// ------ American Put Option -------
class AmericanPutOption : public Option {
private:
    double K_;
public:
    AmericanPutOption(double strike, double maturity)
        : Option(maturity), K_(strike) {}
    double payoff(const std::vector<double>& path) const override {
        if (path.empty()) throw std::invalid_argument("Path is empty");
        double maxPayoff = 0.0;
        for (double price : path) {
            double intrinsic = std::max(K_ - price, 0.0);
            if (intrinsic > maxPayoff) maxPayoff = intrinsic;
        }
        return maxPayoff;
    }
};

#endif