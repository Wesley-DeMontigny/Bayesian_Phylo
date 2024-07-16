#include "MoveScaleBranch.hpp"
#include "TreeObject.hpp"
#include "RandomVariable.hpp"
#include "Node.hpp"
#include <cmath>
#include <iostream>

MoveScaleBranch::MoveScaleBranch(TreeParameter* t) : param(t), delta(std::log(4.0)) {}
        
double MoveScaleBranch::update(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    TreeObject* tree = param->getTree();
    std::vector<Node*> nodes = tree->getPostOrderSeq();
    Node* root = tree->getRoot();

    Node* p = nullptr;
    do{
        p = nodes[(int)(rng.uniformRv() * nodes.size())];
    }
    while(p == root);

    double currentV = tree->getBranchLength(p, p->getAncestor());
    double scale = std::exp(delta * (rng.uniformRv() - 0.5));
    double newV = currentV * scale;
    tree->setBranchLength(p, p->getAncestor(), newV);
    p->setNeedsTPUpdate(true);
    p->flipTP();

    Node* q = p;
    do{
        q->setNeedsCLUpdate(true);
        q->flipCL();
        q = q->getAncestor();
    } 
    while(q != root);
    root->setNeedsCLUpdate(true);
    root->flipCL();

    return std::log(scale);
}

void MoveScaleBranch::accept(){
    param->accept();
    acceptedCount++;
    acceptedSinceTune++;
    countSinceTune++;
}

void MoveScaleBranch::reject(){
    param->reject();
    rejectedCount++;
    countSinceTune++;
}

void MoveScaleBranch::tune(){
    double rate = acceptedSinceTune/countSinceTune;

    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766));
    }
    else {
        delta /= (2.0 - rate/0.234);
    }

    acceptedSinceTune = 0;
    countSinceTune = 0;
}