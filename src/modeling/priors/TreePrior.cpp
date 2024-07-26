#include "TreePrior.hpp"
#include "modeling/parameters/trees/TreeParameter.hpp"
#include "modeling/parameters/trees/Node.hpp"
#include "modeling/parameters/DoubleParameter.hpp"
#include <cmath>

TreePrior::TreePrior(TreeParameter* t) : currentPrior(0.0), oldPrior(0.0), lambda(nullptr), tree(t) {
    this->addChild(t);
    this->dirty();
}

void TreePrior::setExponentialBranchPrior(DoubleParameter* l){
    addChild(l);
    lambda = l;
}

void TreePrior::addMonophyleticConstraint(std::set<int> taxa, double strength){
    //Implement later
}

void TreePrior::accept() {
    oldLnPrior = currentLnPrior;
    for(ModelNode* c : this->getChildren()){
         c->accept();
         c->clean();
    }
}

void TreePrior::reject() {
    currentLnPrior = oldLnPrior;
    for(ModelNode* c : this->getChildren()){
        c->reject();
        c->clean();
    }
}

void TreePrior::regenerate(){
    for(ModelNode* c : this->getChildren()){
        if(c->isDirty())
            this->dirty();
        c->regenerate();
    }

    if(this->isDirty()){

        double totalLnL = 0.0;

        if(lambda != nullptr){
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