#include "TreePrior.hpp"
#include "modeling/parameters/trees/TreeParameter.hpp"
#include "modeling/parameters/trees/Node.hpp"
#include "modeling/parameters/DoubleParameter.hpp"
#include <cmath>
#include <iostream>

TreePrior::TreePrior(TreeParameter* t) : currentPrior(0.0), oldPrior(0.0), lambda(nullptr), tree(t) {
    this->dirty();
}

void TreePrior::setExponentialBranchPrior(DoubleParameter* l){
    lambda = l;
}

void TreePrior::addMonophyleticConstraint(std::set<int> taxa, double strength){
    //Implement later
}

void TreePrior::accept() {
    oldLnPrior = currentLnPrior;
    tree->accept();
    tree->clean();
    if(lambda != nullptr){
        lambda->accept();
        lambda->clean();
    }
}

void TreePrior::reject() {
    currentLnPrior = oldLnPrior;
    tree->reject();
    tree->clean();
    if(lambda != nullptr){
        lambda->reject();
        lambda->clean();
    }
}

void TreePrior::regenerate(){
    tree->regenerate();

    //This isn't pretty. The tree prior should eventually be split between a branch length and topology prior.
    bool hasLambda = lambda != nullptr;
    if(hasLambda){
        lambda->regenerate();
        if(tree->isDirty() || lambda->isDirty())
            this->dirty();
    }
    else{
        if(tree->isDirty())
            this->dirty();
    }
    

    if(this->isDirty()){

        double totalLnL = 0.0;

        if(hasLambda){
            std::vector<double*> values = tree->getBranchLengths();
            double lambdaVal = lambda->getValue();
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
}