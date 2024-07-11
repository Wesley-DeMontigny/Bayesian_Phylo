#include "TreeParameter.hpp"
#include "RandomVariable.hpp"
#include "TreeObject.hpp"

TreeParameter::TreeParameter(TreeObject* t){
    trees[0] = new TreeObject(*t);
    trees[1] = new TreeObject(*trees[0]);//The "memory" of the tree
}

TreeParameter::~TreeParameter(){
    delete trees[0];
    delete trees[1]; 
}

void TreeParameter::accept(){
    *trees[1] = *trees[0];
}


void TreeParameter::reject(){
    *trees[0] = *trees[1];
}