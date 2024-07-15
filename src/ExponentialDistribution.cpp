#include "ExponentialDistribution.hpp"
#include <cmath>

ExponentialDistribution::ExponentialDistribution(double* l, double* p) : lambda(l), values({p}), doubleFunction(nullptr) {}

ExponentialDistribution::ExponentialDistribution(double l, double* p) : lambda(new double(l)), values({p}), doubleFunction(nullptr) {}

ExponentialDistribution::ExponentialDistribution(double* l, std::vector<double*> p) : lambda(l), values(p), doubleFunction(nullptr) {}

ExponentialDistribution::ExponentialDistribution(double l, std::vector<double*> p) : lambda(new double(l)), values(p), doubleFunction(nullptr) {}

ExponentialDistribution::ExponentialDistribution(double l, std::function<std::vector<double*>()> func) : lambda(new double(l)), doubleFunction(func) {

}

ExponentialDistribution::ExponentialDistribution(double* l, std::function<std::vector<double*>()> func) : lambda(l), doubleFunction(func){

}

ExponentialDistribution::~ExponentialDistribution(){
    delete lambda;
}

double ExponentialDistribution::lnLikelihood(){
    if(doubleFunction != nullptr)
        values = doubleFunction();

    double totalL = 1.0;

    for(double* val : values){
        double L = 0;
        if(val >= 0)
            L = (*lambda) * std::exp(-1 * (*lambda) * (*val));
        totalL *= L;
    }

    return std::log(totalL);
}