#include "TreePrior.hpp"
#include "TreeParameter.hpp"
#include "Node.hpp"
#include <cmath>

TreePrior::TreePrior(TreeParameter* t) : tree(t), currentPrior(0.0), oldPrior(0.0) {
    t->setPrior(this);
}

void TreePrior::setExponentialBranchPrior(double l){
    lambda = l;
}

void TreePrior::addMonophyleticConstraint(std::set<int> taxa, double strength){
    //Implement later
}

void TreePrior::regeneratePrior(){
    double totalLnL = 0.0;

    if(lambda != -1.0){
        std::vector<double*> values = tree->getBranchLengths();

        for(double* val : values){
            double LnL = 0.0;
            if(val >= 0)
                LnL = std::log((lambda) * std::exp(-1 * (lambda) * (*val)));
            totalLnL += LnL;
        }

    }
    
    currentPrior = totalLnL;
}