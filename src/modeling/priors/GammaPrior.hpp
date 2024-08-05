#ifndef GAMMA_PRIOR_HPP
#define GAMMA_PRIOR_HPP
#include "PriorNode.hpp"
#include <vector>
#include <set>

class DoubleParameter;

class GammaPrior : public PriorNode {
    public:
        GammaPrior(void)=delete;
        GammaPrior(DoubleParameter* a, DoubleParameter* b, DoubleParameter* p);
        double lnPrior() {return currentLnPrior;}
        void regenerate();
        void accept();
        void reject();
        void sample();
        std::string writeValue() {return std::to_string(currentLnPrior);}
    protected:
        double oldLnPrior;
        double currentLnPrior;
        DoubleParameter* shape;
        DoubleParameter* rate;
        DoubleParameter* param;
};

#endif