#include <iostream>
#include <iomanip>
#include "PricingMC.hpp"


#include <cmath>
#include <tuple>
#include <algorithm>

// Fonction de la loi normale cumulative standard
double N(double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

// Black-Scholes price pour un call européen
std::pair<double ,double> blackScholesCall(
    double S,     // spot
    double K,     // strike
    double r,     // taux sans risque
    double sigma, // volatilité
    double T   ,   // maturité
    double t  
) {
    double d1 = (std::log(S/K) + (r + 0.5*sigma*sigma)*(T - t)) / (sigma * std::sqrt(T - t));
    double d2 = d1 - sigma * std::sqrt(T - t);

    double price = S * N(d1) - K * std::exp(-r*(T - t)) * N(d2);

    double delta = N(d1);

    return  {price , delta};
}

int main() {
    // Paramètres
    double S0 = 100.0;
    double K = 100.0;
    double r = 0.02;
    double sigma = 0.25;
    double T = 1.0; 
    int nSteps = 252;
    int nPaths = 100000; 
    double t = 0.;
    double h =  0.01; // 1%

    CallVanillaOption option(K , T );
    // LookBackCallOption option(T );

    BSModel model(r , sigma);
    PricingMC mc(option , model , nPaths ,nSteps , S0);


    // compute the price and the demi_ic : 
    double price ;
    double demi_ic;
    std::vector<double> past ; 
    mc.price(price , demi_ic , t , past);

    double delta = mc.delta(t , past , h);

    std::cout << "The price of the call option is " << price << " with demi_ic : " << demi_ic << std::endl;
    std::cout << "delta of option is :" << delta <<  std::endl;
    

    // Test : 
    auto [price_bs , delta_bs] = blackScholesCall(S0, K, r, sigma, T , t);
    std::cout << "The price of the call option with BS Model is "  <<  price_bs <<  std::endl;
    std::cout << "delta of option is :" << delta_bs <<  std::endl;

}
