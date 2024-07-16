#include "ExponentialDistribution.hpp"
#include <cmath>

ExponentialDistribution::ExponentialDistribution(double l, double* p) : lambda(l), values({p}), doubleFunction(nullptr) {}

ExponentialDistribution::ExponentialDistribution(double l, std::vector<double*> p) : lambda(l), values(p), doubleFunction(nullptr) {}

ExponentialDistribution::ExponentialDistribution(double l, std::function<std::vector<double*>()> func) : lambda(l), doubleFunction(func) {}

ExponentialDistribution::~ExponentialDistribution() {}

double ExponentialDistribution::lnLikelihood(){
    if(doubleFunction != nullptr)
        values = doubleFunction();

    double totalLnL = 1.0;

    for(double* val : values){
        double LnL = 0;
        if(val >= 0)
            LnL = std::log(lambda * std::exp(-1 * lambda * (*val)));
        totalLnL += LnL;
    }

    return totalLnL;
}