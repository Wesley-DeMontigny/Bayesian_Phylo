#ifndef NORMAL_PRIOR_HPP
#define NORMAL_PRIOR_HPP
#include "PriorNode.hpp"
#include <vector>
#include <set>

class DoubleParameter;

class NormalPrior : public PriorNode {
    public:
        NormalPrior(void)=delete;
        NormalPrior(DoubleParameter* m, DoubleParameter* s, DoubleParameter* p);
        double lnPrior() {return currentLnPrior;}
        void regenerate();
        void accept();
        void reject();
        void sample();
        std::string writeValue() {return std::to_string(currentLnPrior);}
    protected:
        double oldLnPrior;
        double currentLnPrior;
        DoubleParameter* mu;
        DoubleParameter* sigma;
        DoubleParameter* param;
};

#endif