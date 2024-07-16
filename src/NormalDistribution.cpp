#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
#endif
#include "NormalDistribution.hpp"
#include <cmath>

NormalDistribution::NormalDistribution(double m, double s, double* p) : mu(m), sigma(s), values({p}), doubleFunction(nullptr) {}

NormalDistribution::NormalDistribution(double m, double s, std::vector<double*> p) : mu(m), sigma(s), values(p), doubleFunction(nullptr) {}

NormalDistribution::NormalDistribution(double m, double s, std::function<std::vector<double*>()> func) : mu(m), sigma(s), doubleFunction(func) {}

NormalDistribution::~NormalDistribution() {}

double NormalDistribution::lnLikelihood() {
    if(doubleFunction != nullptr)
        values = doubleFunction();

    double totalLnL = 1.0;

    for(double* val : values){
        double LnL = 0;
        if(val >= 0)
            LnL = std::log((1/std::sqrt(2 * M_PI * std::pow(sigma, 2))) * 
            exp(-1.0 * std::pow(*val - mu, 2)/(2 * std::pow(sigma, 2))));
        totalLnL += LnL;
    }

    return totalLnL;
}