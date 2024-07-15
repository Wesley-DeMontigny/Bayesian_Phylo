#ifndef EXPONENTIAL_DISTRIBUTION_HPP
#define EXPONENTIAL_DISTRIBUTION_HPP
#include "AbstractDistribution.hpp"
#include <vector>
#include <functional>

class ExponentialDistribution : public AbstractDistribution{
    public:
        ExponentialDistribution(void)=delete;
        ExponentialDistribution(double l, double* p);
        ExponentialDistribution(double* l, double* p);
        ExponentialDistribution(double l, std::vector<double*> p);
        ExponentialDistribution(double* l, std::vector<double*> p);
        ExponentialDistribution(double l, std::function<std::vector<double*>()> func);
        ExponentialDistribution(double* l, std::function<std::vector<double*>()> func);
        ~ExponentialDistribution();
        double lnLikelihood();
    private:
        std::function<std::vector<double*>()> doubleFunction;
        double* lambda;
        std::vector<double*> values;
};

#endif;