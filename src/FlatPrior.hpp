#ifndef FLAT_PRIOR_HPP
#define FLAT_PRIOR_HPP
#include "AbstractDistribution.hpp"

class FlatPrior : public AbstractDistribution{
    public:
        FlatPrior(void){};
        double lnLikelihood() {return 0.0;}
    private:
};

#endif