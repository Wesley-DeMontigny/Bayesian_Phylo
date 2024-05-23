#include "Tree.hpp"
#include "RandomVariable.hpp"
#include "Node.hpp"
#include "Msg.hpp"
#include <iostream>

/*
=======================================================================
                        BASIC TREE FUNCTIONALITY
=======================================================================
*/

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
    
    // Initialize branch lengths
    for (int i=0, n=(int)postOrderSeq.size(); i<n; i++)
        {
        Node* p = postOrderSeq[i];
        if (p->getAncestor() != nullptr)
            this->setBranchLength(p,p->getAncestor(), rng->exponentialRv(10.0));
        }

    // index the interior nodes (the tip nodes are indexed, above)
    int intIdx = numTaxa;
    for (int i=0, n=(int)postOrderSeq.size(); i<n; i++)
        {
        Node* p = postOrderSeq[i];
        if (p->getIsTip() == false)
            p->setIndex(intIdx++);
        }
}

Tree::Tree(const Tree& t){
    clone(t);
}

Tree::~Tree(void) {
    deleteAllNodes();
}

void Tree::deleteAllNodes(){
    for (int i = 0; i < nodes.size(); i++)
        delete nodes[i];
    nodes.clear();
}

Tree& Tree::operator=(const Tree& rhs){
    if(this == &rhs)
        return *this;
    
    clone(rhs);
    return *this;
}

void Tree::clone(const Tree& t){
    if(nodes.size() != t.nodes.size()){
        deleteAllNodes();
        for(int i = 0; i < t.nodes.size(); i++)
            addNode();
    }

    this->numTaxa = t.numTaxa;
    this->root = this->nodes[t.root->getOffset()];

    for(int i = 0; i < t.nodes.size(); i++){
        Node* p = this->nodes[i];
        Node* q = t.nodes[i];
        p->setIndex(q->getIndex());
        p->setIsTip(q->getIsTip());
        p->setName(q->getName());

        if(q->getAncestor() != nullptr){
            Node* ancestor = this->nodes[q->getAncestor()->getOffset()];
            double bl = t.getBranchLength(q, q->getAncestor());
            p->setAncestor(ancestor);
            setBranchLength(p, ancestor, bl);
        }
        else
            p->setAncestor(nullptr);

        p->removeAllNeighbors();
        std::set<Node*>& qNeighbors = q->getNeighbors();
        for(Node* n : qNeighbors)
            p->addNeighbor(this->nodes[n->getOffset()]);
    }

    this->postOrderSeq.clear();
    for(int i = 0; i < t.postOrderSeq.size(); i++){
        Node* p = t.postOrderSeq[i];
        this->postOrderSeq.push_back(this->nodes[p->getOffset()]);
    }
}

Node* Tree::addNode(void) {

    Node* newNode = new Node;
    newNode->setOffset((int)nodes.size());
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

void Tree::setBranchLength(Node* p1, Node* p2, double length){
    std::pair<Node*, Node*> key;
    if(p1 < p2)
        key = std::make_pair(p1,p2);
    else
        key = std::make_pair(p2, p1);
    std::map<std::pair<Node*, Node*>, double>::iterator it = branchLengths.find(key);

    if(it == branchLengths.end())
        branchLengths.insert(std::make_pair(key, length));
    else
        it->second = length;
}

double Tree::getBranchLength(Node* p1, Node* p2) const{
    std::pair<Node*, Node*> key;
    if(p1 < p2)
        key = std::make_pair(p1,p2);
    else
        key = std::make_pair(p2, p1);
    std::map<std::pair<Node*, Node*>, double>::const_iterator it = branchLengths.find(key);

    if(it == branchLengths.end())
        Msg::error("Couldn't find branch length of pair");
    return it->second;
}

/*
=======================================================================
                            TREE OUTPUT
=======================================================================
*/

std::string Tree::getNewick() const{
    std::stringstream strm;
    writeNode(root, strm);
    strm << ";";
    return strm.str();
}


void Tree::print(std::string header) const{
    std::cout << header << std::endl;
    print();
}

void Tree::print(void) const{

    showNode(root, 0);
}

//Output nodes of the tree in a whitespace-indented format
void Tree::showNode(Node* p, int indent) const{

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

    if(p->getAncestor() != nullptr)
        std::cout << this->getBranchLength(p, p->getAncestor()) << " ";

    std::cout << p->getName();

    if (p == root)
        std::cout << " <- Root";
    std::cout << std::endl;

    for(Node* n : pNeighbors)
        {
        if(n != p->getAncestor())
            showNode(n, indent+3);
        }
}

//For outputting a newick string
void Tree::writeNode(Node* p, std::stringstream& strm) const{
    if(p == nullptr)
        return;
    
    if(!p->getIsTip())
        strm << "(";
    else
        strm << p->getName();

    std::set<Node*>& pDesc = p->getNeighbors();
    bool foundFirst = false;
    for(Node* n : pDesc){
        if(n != p->getAncestor()){
            if(foundFirst)
                strm << ",";
            foundFirst = true;
            writeNode(n, strm);
        }
    }

    if(!p->getIsTip())
        strm << ")";
    if(p->getAncestor() != nullptr){
        strm << ":" << this->getBranchLength(p, p->getAncestor());
    }
}
