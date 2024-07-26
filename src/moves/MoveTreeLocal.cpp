#include "MoveTreeLocal.hpp"
#include "modeling/parameters/trees/TreeObject.hpp"
#include "MoveScheduler.hpp"
#include "core/RandomVariable.hpp"
#include "modeling/parameters/trees/Node.hpp"
#include <iostream>
#include <cmath>

MoveTreeLocal::MoveTreeLocal(TreeParameter* t) : param(t) {}
        
/*
See Larget and Simon (1999) for original description of the move and Mark Holder
et al. (2005) for the correction on the Hastings ratio.

The move is basically as follows:
1. Pick an internal branch and two neighbor nodes of each of the nodes of the branch
2. Scale the length of that whole path like you would normally do.
3. Randomly draw a position on that length to pick off the subtree originally connected
   to one of the internal nodes and graft it there.
*/
double MoveTreeLocal::update(){

    TreeObject* tree = param->getTree();
    std::vector<Node*> nodes = tree->getPostOrderSeq();
    Node* root = tree->getRoot();

    RandomVariable& rng = RandomVariable::randomVariableInstance();

    Node* u = nullptr;
    do{
        u = nodes[(int)(rng.uniformRv() * nodes.size())];
    }
    while(u == root || u->getIsTip() == true);
    Node* v = u->getAncestor();

    std::set<Node*> neighbors1 = u->getNeighbors();
    neighbors1.erase(v);//Exclude v
    Node* a = Node::chooseNodeFromSet(neighbors1);

    std::set<Node*> neighbors2 = v->getNeighbors();
    neighbors2.erase(u);//Don't select u
    Node* c = Node::chooseNodeFromSet(neighbors2);

    double scale = std::exp(delta * (rng.uniformRv() - 0.5));
    double paths[3] {
        tree->getBranchLength(u, v) * scale,
        tree->getBranchLength(a, u) * scale,
        tree->getBranchLength(v, c) * scale
    };
    double totalPath = paths[0] + paths[1] + paths[2];

    std::vector<Node*> setUV = {u, v};
    std::vector<Node*> setAC = {a, c};
    int pick = (int)(rng.uniformRv() * 2);
    double randomLoc = rng.uniformRv() * totalPath;

    //The pick decides the oritentation of the path
    if(randomLoc <= totalPath - paths[pick]){
        tree->setBranchLength(setAC[pick], setUV[pick], randomLoc);
        tree->setBranchLength(u, v, totalPath - paths[pick] - randomLoc);
        tree->setBranchLength(setAC[pick ^ 1], setUV[pick ^ 1], paths[pick]);
    }
    else{
        u->removeNeighbor(a);
        a->removeNeighbor(u);
        v->removeNeighbor(c);
        c->removeNeighbor(v);
        tree->removeBranchLength(a, u);
        tree->removeBranchLength(v,c);

        v->addNeighbor(a);
        a->addNeighbor(v);
        u->addNeighbor(c);
        c->addNeighbor(u);
        tree->setBranchLength(setAC[pick ^ 1], setUV[pick], totalPath - randomLoc);
        tree->setBranchLength(u, v, randomLoc - (totalPath - paths[pick]));
        tree->setBranchLength(setAC[pick], setUV[pick ^ 1], totalPath - paths[pick]);

        //Rooting logic
        if(v->getAncestor() == c){
            u->setAncestor(c);
            v->setAncestor(u);
            a->setAncestor(v);
        }
        else{
            c->setAncestor(u);
            u->setAncestor(v);
            a->setAncestor(v);
        }
    }


    u->setNeedsTPUpdate(true);
    v->setNeedsTPUpdate(true);

    a->setNeedsTPUpdate(true);
    c->setNeedsTPUpdate(true);

    //The updating gets a little awkward because we don't really know the branching here.
    Node* q = v;
    if(v->getAncestor() != u)
        q = u;

    do{
        q->setNeedsCLUpdate(true);
        q = q->getAncestor();
    }
    while(q != root);
    root->setNeedsCLUpdate(true);

    tree->initPostOrder();

    param->dirty();

    return std::pow(scale, 3);
}

void MoveTreeLocal::tune(){
    //Something about this causes the branch lengths to go to 0
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