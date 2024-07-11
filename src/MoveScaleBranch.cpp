#include "MoveScaleBranch.hpp"
#include "TreeObject.hpp"
#include "RandomVariable.hpp"
#include "Node.hpp"
#include <cmath>

MoveScaleBranch::MoveScaleBranch(TreeParameter* t) : param(t) {}
        
double MoveScaleBranch::update(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    TreeObject* tree = param->getValue();
    std::vector<Node*> nodes = tree->getPostOrderSeq();
    Node* root = tree->getRoot();

    Node* p = nullptr;
    do{
        p = nodes[(int)(rng.uniformRv() * nodes.size())];
    }
    while(p == root);

    double currentV = tree->getBranchLength(p, p->getAncestor());
    double tuning = std::log(4.0);
    double newV = currentV * exp(tuning * (rng.uniformRv() - 0.5));
    tree->setBranchLength(p, p->getAncestor(), newV);
    p->setNeedsTPUpdate(true);

    Node* q = p;
    do{
        q->setNeedsCLUpdate(true);
        q = q->getAncestor();
    } 
    while(q != root);
    root->setNeedsCLUpdate(true);

    return std::log(newV/currentV);
}