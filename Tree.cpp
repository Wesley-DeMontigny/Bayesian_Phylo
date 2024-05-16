#include "Tree.hpp"
#include "RandomVariable.hpp"
#include "Node.hpp"
#include <iostream>

Tree::Tree(RandomVariable* rng, int nt) : numTaxa(nt) {

    // initialize an unrooted three-taxon tree, (0,1,2)
    root = addNode();
    root->setName("Root");
    for(int i = 0; i < 3; i++)
        {
        Node* p = addNode();
        p->setName("Taxon_" + std::to_string(i+1));
        p->setIsTip(true);
        p->setIndex(i);
        p->addNeighbor(root);
        root->addNeighbor(p);
        p->setAncestor(root);
        }

    // build up the full tree by randomly adding branches to the existing tree
    for(int i = 3; i < numTaxa; i++)
        {
        Node* p = nullptr;
        do {
            p = nodes[(int)(rng->uniformRv() * nodes.size())];
        } while(p == root);

        Node* pAnc = p->getAncestor();
        Node* newTip = addNode();
        newTip->setIsTip(true);
        newTip->setIndex(i);
        newTip->setName("Taxon_" + std::to_string(i+1));
        Node* newAnc = addNode();

        p->removeNeighbor(pAnc);
        p->addNeighbor(newAnc);
        p->setAncestor(newAnc);

        newAnc->addNeighbor(p);
        newAnc->addNeighbor(newTip);
        newAnc->addNeighbor(pAnc);
        newAnc->setAncestor(pAnc);

        newTip->addNeighbor(newAnc);
        newTip->setAncestor(newAnc);

        pAnc->removeNeighbor(p);
        pAnc->addNeighbor(newAnc);
        }
        
    // initialize the down pass sequence
    initPostOrder();
    
    // index the interior nodes (the tip nodes are indexed, above)
    int intIdx = numTaxa;
    for (int i=0, n=(int)postOrderSeq.size(); i<n; i++)
        {
        Node* p = postOrderSeq[i];
        if (p->getIsTip() == false)
            p->setIndex(intIdx++);
        }
}

Tree::~Tree(void) {

    for (int i = 0; i < nodes.size(); i++)
        delete nodes[i];
}

Node* Tree::addNode(void) {

    Node* newNode = new Node;
    nodes.push_back(newNode);
    return newNode;
}

void Tree::initPostOrder(void) {

    postOrderSeq.clear();
    passDown(root, root);
}

void Tree::passDown(Node* p, Node* fromNode) {

    if(p == nullptr)
        return;
    
    std::set<Node*>& pNeighbors = p->getNeighbors();

    for(Node* n : pNeighbors)
        {
        if(n != p->getAncestor())
            passDown(n, p);
        }

    if(p != fromNode)
        p->setAncestor(fromNode);

    postOrderSeq.push_back(p);
}

void Tree::print(void) {

    showNode(root, 0);
}

void Tree::showNode(Node* p, int indent) {

    if(p == nullptr)
        return;

    for(int i = 0; i < indent; i++)
        std::cout << " ";

    std::cout << p->getIndex() << " ( ";
    std::set<Node*>& pNeighbors = p->getNeighbors();
    for (Node* d : pNeighbors)
        {
        if (d == p->getAncestor())
            std::cout << "a_";
        std::cout << d->getIndex() << " ";
        }
    std::cout << ") ";
    if (p == root)
        std::cout << " <- Root";
    std::cout << std::endl;

    for(Node* n : pNeighbors)
        {
        if(n != p->getAncestor())
            showNode(n, indent+3);
        }
}
