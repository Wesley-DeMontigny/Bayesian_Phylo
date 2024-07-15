#ifndef COMPOUND_DISTRIBUTION_HPP
#define COMPOUND_DISTRIBUTION_HPP
#include "AbstractDistribution.hpp"
#include <vector>

class CompoundDistribution : public AbstractDistribution{
    public:
        CompoundDistribution(void);
        CompoundDistribution(std::vector<AbstractDistribution*> d);
        void addDistribution(AbstractDistribution* d);
        double lnLikelihood();
    private:
        std::vector<AbstractDistribution*> distributions;
};

#endif