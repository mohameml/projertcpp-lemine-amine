#include "Option.hpp"
#include <algorithm>
#include <stdexcept>

// =================== CallVanillaOption : ================
CallVanillaOption::CallVanillaOption(double strike, double maturity)
    : Option(maturity), K_(strike) {}

double CallVanillaOption::payoff(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::invalid_argument("Path is empty");
    }
    double S_T = path.back();  // final price on path
    return std::max(S_T - K_, 0.0);
}

// ==================== PutVanillaOption : =================
PutVanillaOption::PutVanillaOption(double strike, double maturity)
    : Option(maturity), K_(strike) {}

double PutVanillaOption::payoff(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::invalid_argument("Path is empty");
    }
    double S_T = path.back();
    return std::max(K_ - S_T, 0.0);
}

// ==================== LookBackCallOption : =================
LookBackCallOption::LookBackCallOption(double maturity)
    : Option(maturity) {}

double LookBackCallOption::payoff(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::invalid_argument("Path is empty");
    }
    double S_T = path.back();
    double minPrice = *std::min_element(path.begin(), path.end());
    return std::max(S_T - minPrice, 0.0);
}

// ==================== LookBackPutOption : =================
LookBackPutOption::LookBackPutOption(double maturity)
    : Option(maturity) {}

double LookBackPutOption::payoff(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::invalid_argument("Path is empty");
    }
    double S_T = path.back();
    double maxPrice = *std::max_element(path.begin(), path.end());
    return std::max(maxPrice - S_T, 0.0);
}
// Digital Call Option
class DigitalCallOption : public Option {
private:
    double K_;
    double payout_;

public:
    DigitalCallOption(double strike, double maturity, double payout = 1.0)
        : Option(maturity), K_(strike), payout_(payout) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Path is empty");
        return (path.back() > K_) ? payout_ : 0.0;
    }
};

// Digital Call Option
class DigitalCallOption : public Option {
private:
    double K_;
    double payout_;

public:
    DigitalCallOption(double strike, double maturity, double payout = 1.0)
        : Option(maturity), K_(strike), payout_(payout) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Path is empty");
        return (path.back() > K_) ? payout_ : 0.0;
    }
};

// Digital Put Option
class DigitalPutOption : public Option {
private:
    double K_;
    double payout_;

public:
    DigitalPutOption(double strike, double maturity, double payout = 1.0)
        : Option(maturity), K_(strike), payout_(payout) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Path is empty");
        return (path.back() < K_) ? payout_ : 0.0;
    }
};

// Asian Call Option
class AsianCallOption : public Option {
private:
    double K_;

public:
    AsianCallOption(double strike, double maturity)
        : Option(maturity), K_(strike) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Path is empty");
        double avg = std::accumulate(path.begin(), path.end(), 0.0) / path.size();
        return std::max(avg - K_, 0.0);
    }
};

// Asian Put Option
class AsianPutOption : public Option {
private:
    double K_;

public:
    AsianPutOption(double strike, double maturity)
        : Option(maturity), K_(strike) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Path is empty");
        double avg = std::accumulate(path.begin(), path.end(), 0.0) / path.size();
        return std::max(K_ - avg, 0.0);
    }
};

// American Call Option (approximates payoff by max intrinsic over path)
class AmericanCallOption : public Option {
private:
    double K_;

public:
    AmericanCallOption(double strike, double maturity)
        : Option(maturity), K_(strike) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Path is empty");

        double maxPayoff = 0.0;
        for (double price : path) {
            double intrinsic = std::max(price - K_, 0.0);
            if (intrinsic > maxPayoff) maxPayoff = intrinsic;
        }
        return maxPayoff;
    }
};

// American Put Option (approximates payoff by max intrinsic over path)
class AmericanPutOption : public Option {
private:
    double K_;

public:
    AmericanPutOption(double strike, double maturity)
        : Option(maturity), K_(strike) {}

    double payoff(const std::vector<double>& path) const override {
        if (path.empty())
            throw std::invalid_argument("Path is empty");

        double maxPayoff = 0.0;
        for (double price : path) {
            double intrinsic = std::max(K_ - price, 0.0);
            if (intrinsic > maxPayoff) maxPayoff = intrinsic;
        }
        return maxPayoff;
    }
};
