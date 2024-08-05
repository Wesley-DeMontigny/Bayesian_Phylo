#ifndef EXPONENTIAL_PRIOR_HPP
#define EXPONENTIAL_PRIOR_HPP
#include "PriorNode.hpp"
#include <vector>
#include <set>

class DoubleParameter;

class ExponentialPrior : public PriorNode {
    public:
        ExponentialPrior(void)=delete;
        ExponentialPrior(DoubleParameter* l, DoubleParameter* p);
        double lnPrior() {return currentLnPrior;}
        void regenerate();
        void accept();
        void reject();
        void sample();
        std::string writeValue() {return std::to_string(currentLnPrior);}
    protected:
        double oldLnPrior;
        double currentLnPrior;
        DoubleParameter* lambda;
        DoubleParameter* param;
};

#endif