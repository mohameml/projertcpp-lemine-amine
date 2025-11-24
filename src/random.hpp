#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

#include <random>

class RandomGenerator {
public : 
    explicit RandomGenerator(uint64_t seed = std::random_device{}())
        : rng_(seed) , nd_(0.0 , 1.0) {}

    double normal() {
        return nd_(rng_);
    }
    

private :
    std::mt19937_64 rng_;
    std::normal_distribution<double> nd_;
};


#endif 
