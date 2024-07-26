#include "TreeParameter.hpp"
#include "core/RandomVariable.hpp"
#include "TreeObject.hpp"

TreeParameter::~TreeParameter(){
    delete trees[0];
    delete trees[1]; 
}

void TreeParameter::accept(){
    *trees[1] = *trees[0];
}

std::vector<double*> TreeParameter::getBranchLengths(){
    return trees[0]->getBranchLengths();
}

void TreeParameter::reject(){
    *trees[0] = *trees[1];
}

void TreeParameter::regenerate() {}

std::string TreeParameter::writeValue(){
    return trees[0]->getNewick();
}