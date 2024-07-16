#ifndef NORMAL_DISTRIBUTION_HPP
#define NORMAL_DISTRIBUTION_HPP
#include "AbstractDistribution.hpp"
#include <vector>
#include <functional>

class NormalDistribution : public AbstractDistribution{
    public:
        NormalDistribution(void)=delete;
        NormalDistribution(double m, double s, double* p);
        NormalDistribution(double m, double s, std::vector<double*> p);
        NormalDistribution(double m, double s, std::function<std::vector<double*>()> func);
        ~NormalDistribution();
        double lnLikelihood();
    private:
        std::function<std::vector<double*>()> doubleFunction;
        double mu;
        double sigma;
        std::vector<double*> values;
};

#endif