#ifndef TREE_PRIOR_HPP
#define TREE_PRIOR_HPP
#include "PriorNode.hpp"
#include <vector>
#include <set>

class TreeParameter;
class DoubleParameter;

class TreePrior : public PriorNode {
    public:
        TreePrior(void)=delete;
        TreePrior(TreeParameter* t);
        void setExponentialBranchPrior(DoubleParameter* l);
        void addMonophyleticConstraint(std::set<int> taxa, double strength);
        double lnPrior() {return currentPrior;}
        void regenerate();
        void accept();
        void reject();
        void sample();
        std::string writeValue() {return std::to_string(currentPrior);}
    protected:
        double oldPrior;
        double currentPrior;
        DoubleParameter* lambda;
        TreeParameter* tree;
        std::vector<std::set<int>> monophyleticContraints;
};

#endif