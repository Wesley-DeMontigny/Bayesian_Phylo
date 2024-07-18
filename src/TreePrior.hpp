#ifndef TREE_PRIOR_HPP
#define TREE_PRIOR_HPP
#include "AbstractPrior.hpp"
#include <vector>
#include <set>

class TreeParameter;

class TreePrior : public AbstractPrior {
    public:
        TreePrior(void)=delete;
        TreePrior(TreeParameter* t);
        void setExponentialBranchPrior(double l);
        void addMonophyleticConstraint(std::set<int> taxa, double strength);
        double lnPrior() {return currentPrior;}
        void regeneratePrior();
        void acceptPrior() {oldPrior = currentPrior;}
        void rejectPrior() {currentPrior = oldPrior;}
    protected:
        double oldPrior;
        double currentPrior;
    private:
        std::vector<std::set<int>> monophyleticContraints;
        TreeParameter* tree;
        double lambda = -1.0;
};

#endif