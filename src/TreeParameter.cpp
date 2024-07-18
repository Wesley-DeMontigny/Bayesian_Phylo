#include "TreeParameter.hpp"
#include "RandomVariable.hpp"
#include "TreeObject.hpp"
#include "AbstractLikelihood.hpp"
#include "AbstractPrior.hpp"

TreeParameter::~TreeParameter(){
    delete trees[0];
    delete trees[1]; 
}

void TreeParameter::accept(){
    *trees[1] = *trees[0];
    if(likelihood != nullptr)
        likelihood->acceptLikelihood();
    if(prior != nullptr)
        prior->acceptPrior();
}

std::vector<double*> TreeParameter::getBranchLengths(){
    return trees[0]->getBranchLengths();
}

void TreeParameter::reject(){
    *trees[0] = *trees[1];
    if(likelihood != nullptr)
        likelihood->acceptLikelihood();
    if(prior != nullptr)
        prior->acceptPrior();
}

void TreeParameter::regenerate(){
    if(prior != nullptr)
        prior->regeneratePrior();
    if(likelihood != nullptr)
        likelihood->regenerateLikelihood();
}

std::string TreeParameter::writeValue(){
    return trees[0]->getNewick();
}