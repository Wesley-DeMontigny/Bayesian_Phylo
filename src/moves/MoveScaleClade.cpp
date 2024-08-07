#include "MoveScaleClade.hpp"
#include "modeling/parameters/trees/TreeObject.hpp"
#include "MoveScheduler.hpp"
#include "core/RandomVariable.hpp"
#include "modeling/parameters/trees/Node.hpp"
#include <iostream>
#include <cmath>

MoveScaleClade::MoveScaleClade(TreeParameter* t) : param(t) {}
        
double MoveScaleClade::update(){

    TreeObject* tree = param->getTree();
    std::vector<Node*> nodes = tree->getPostOrderSeq();
    Node* root = tree->getRoot();

    RandomVariable& rng = RandomVariable::randomVariableInstance();

    Node* u = nullptr;
    do{
        u = nodes[(int)(rng.uniformRv() * nodes.size())];
    }
    while(u == root || u->getIsTip() == true);

    std::vector<Node*> clade;
    tree->passDown(u, clade);

    double scale = std::exp(delta * (rng.uniformRv() - 0.5));
    for(Node* n : clade){
        tree->setBranchLength(n, scale * tree->getBranchLength(n));
        n->setNeedsTPUpdate(true);
        if(n->getIsTip() == false)
            n->setNeedsCLUpdate(true);
    }

    Node* q = u->getAncestor();

    while(q != root){
        q->setNeedsCLUpdate(true);
        q = q->getAncestor();
    }
    root->setNeedsCLUpdate(true);

    tree->initPostOrder();

    param->dirty();

    return scale;
}

void MoveScaleClade::tune(){
    double rate = (double)acceptedSinceTune/(double)countSinceTune;

    if ( rate > 0.44 ) {
        delta *= (1.0 + ((rate-0.44)/0.766));
    }
    else {
        delta /= (2.0 - rate/0.44);
    }
    acceptedSinceTune = 0;
    countSinceTune = 0;
}