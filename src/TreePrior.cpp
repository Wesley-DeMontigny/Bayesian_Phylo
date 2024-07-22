#include "TreePrior.hpp"
#include "TreeParameter.hpp"
#include "Node.hpp"
#include "DoubleParameter.hpp"
#include <cmath>

TreePrior::TreePrior(TreeParameter* t) : tree(t), currentPrior(0.0), oldPrior(0.0), lambda(-1.0), lambdaParam(nullptr) {
    t->setPrior(this);
}

void TreePrior::setExponentialBranchPrior(DoubleParameter* l){
    lambdaParam = l;
    l->setPrior(this);
}

void TreePrior::addMonophyleticConstraint(std::set<int> taxa, double strength){
    //Implement later
}

void TreePrior::regeneratePrior(){
    double totalLnL = 0.0;

    if(lambdaParam != nullptr){
        std::vector<double*> values = tree->getBranchLengths();
        double lambdaVal = lambdaParam->getValue();
        for(double* val : values){
            double LnL = 0.0;
            if(val > 0)
                LnL = std::log(lambdaVal * std::exp(-1 * (lambdaVal) * (*val)));
            else
                LnL = -INFINITY;
            totalLnL += LnL;
        }
    }
    
    currentPrior = totalLnL;
}