#include "Tree.hpp"
#include "RandomVariable.hpp"
#include "Node.hpp"
#include "Msg.hpp"
#include "Alignment.hpp"
#include <iostream>

/*
=======================================================================
                    TREE CONSTRUCTORS/DESTRUCTORS
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

//Generate a random tree and connect it to an alignment
Tree::Tree(RandomVariable* rng, Alignment* aln) : Tree(rng, aln->getNumTaxa()) {

    std::vector<std::string> names = aln->getTaxaNames();
    for(Node* n : postOrderSeq){
        if(n->getIsTip())
            n->setName(names[n->getIndex()]);
    }
    
}

//Change to match index to taxon name
Tree::Tree(std::string newick, std::vector<std::string> taxaNames){
    std::vector<std::string> tokens = parseNewickString(newick);

    Node* p = nullptr;
    bool readingBranchLength = false;

    numTaxa = 0;

    for(std::string tok : tokens){
        if(tok == "("){
            Node* newNode = addNode();
            if(p == nullptr)
                root = newNode;
            else{
                p->addNeighbor(newNode);
                newNode->addNeighbor(p);
                newNode->setAncestor(p);
                setBranchLength(p, newNode, 0.0);
            }

            p = newNode;
        }
        else if(tok == ")" || tok == ","){
            if(p->getAncestor() != nullptr)
                p = p->getAncestor();
            else
                Msg::error("Poorly formatted Newick -P should not be null.");
        }
        else if(tok == ":"){
            readingBranchLength = true;
        }
        else if(tok == ";"){
            if(p != root)
                Msg::error("Poorly formatted Newick - Did not end at root.");
        }
        else{
            if(readingBranchLength){
                double x = atof(tok.c_str());
                setBranchLength(p, p->getAncestor(), x);
            }
            else{
                Node* newNode = addNode();
                p->addNeighbor(newNode);
                newNode->addNeighbor(p);
                newNode->setAncestor(p);
                newNode->setName(tok);
                newNode->setIsTip(true);
                setBranchLength(p, newNode, 0.0);

                int taxonIndex = getTaxonIndex(tok, taxaNames);
                if(taxonIndex == -1)
                    Msg::error("Token is not in taxa names");
                newNode->setIndex(taxonIndex);

                p = newNode;
                numTaxa++;
            }
            readingBranchLength = false;
        }
    }
    initPostOrder();
    
    if(numTaxa != taxaNames.size())
        Msg::error("Taxa names do not match newick string");

    int idx = numTaxa;
    for(Node* n : postOrderSeq){
        if(n->getIsTip() == false){
            n->setIndex(idx++);
        }
    }
}

Tree::Tree(const Tree& t){
    clone(t);
}

/*
-------------- Birth-Death Cladogenesis Tree Constructor ----------------------
The Birth-Death Model of Cladogenesis is a model for speciation that treats
speciation (splitting a branch) and termination (ending of a branch) as random 
evenets. At the end of the simulation, all extant species have equal branch
lengths, while the extinct species have shorter ones corresponding to the time
of extinction.

The probability of speciation during a given interval of time is t*lambda, and 
the probability of extinction during an interval of time is t*mu. Thus, the probability
of an event happening at all (extinction or speciation) is distributed according to
the rate lambda + mu in an exp distribution. Given an event occurs, it's speciation
probability is lambda/(lambda+mu) and the extinction probability is mu/(lambda+mu). 
This makes sense - it is basically the lambda or mu share of lambda+mu.

The algorithm here is pretty simple:
1. Make a loop and during each iteration add a waiting time to the the counter t.
2. So long as the time hasn't passed the maximum duration, draw a uniform variable.
3. If the uniform variable is less than lambda/(lambda+mu) then it corresponds to the
   speciation probability. Pick a random non-dead branch and split it.
4. If the event corresponds with death, kill off the branch.
*/
Tree::Tree(RandomVariable* rng, double lambda, double mu, double duration){

    root = addNode();
    root->setName("Root");

    std::set<Node*> activeNodes;
    /*
    I am tracking whether or not a node is alive or dead within the function
    but hypothetically I could have a derived class that tracks it as a variable
    within the tree. I could imagine that being useful if you actually cared about
    what branches are alive or dead.
    */
    std::map<Node*, double> birthPoints;
    std::map<Node*, double> endPoints;

    Node* n1 = addNode();
    n1->setAncestor(root);
    n1->addNeighbor(root);
    root->addNeighbor(n1);

    
    activeNodes.insert(n1);
    birthPoints.insert({n1, 0.0});

    double t = 0.0;
    while(t < duration){
        double rate = activeNodes.size() * (lambda + mu);
        t += rng->exponentialRv(rate);

        if(t < duration){
            Node* selectedP = chooseNodeFromSet(activeNodes, rng);

            double u = rng->uniformRv();
            if(u < lambda / (lambda+mu)){
                //Speciation
                Node* p1 = addNode();
                p1->setAncestor(selectedP);
                p1->addNeighbor(selectedP);
                selectedP->addNeighbor(p1);
                Node* p2 = addNode();
                p2->setAncestor(selectedP);
                p2->addNeighbor(selectedP);
                selectedP->addNeighbor(p2);

                activeNodes.erase(selectedP);
                activeNodes.insert(p1);
                activeNodes.insert(p2);

                endPoints.insert({selectedP, t});
                birthPoints.insert({p1, t});
                birthPoints.insert({p2, t});
            }
            else{
                //Extinction
                activeNodes.erase(selectedP);
                endPoints.insert({selectedP, t});
            }
        }
    }

    //Set the branch ends for all currently active branches
    for(Node* n : activeNodes)
        endPoints.insert({n, t});

    initPostOrder();

    //Set names, indices, and tips
    int currentID = 0;
    for(int i = 0; i < postOrderSeq.size(); i++){
        Node* p = postOrderSeq[i];

        if(p->getAncestor() != nullptr){
            if(p->getNeighbors().size() == 1){
                p->setIndex(currentID++);
                p->setName("Taxon_" + std::to_string(currentID));
                p->setIsTip(true);
            }
            auto itB = birthPoints.find(p);
            auto itE = endPoints.find(p);

            setBranchLength(p, p->getAncestor(), itE->second - itB->second);
        }
    }
}

Tree::~Tree(void) {
    deleteAllNodes();
}

//Deep Copy Operation
Tree& Tree::operator=(const Tree& rhs){
    if(this == &rhs)
        return *this;
    
    clone(rhs);
    return *this;
}

/*
=======================================================================
                        Other Functions
=======================================================================
*/

Node* Tree::addNode(void) {

    Node* newNode = new Node;
    newNode->setOffset((int)nodes.size());
    nodes.push_back(newNode);
    return newNode;
}

Node* Tree::chooseNodeFromSet(std::set<Node*>& s, RandomVariable* rng){
    int whichNode = (int)(s.size() * rng->uniformRv());
    int i = 0;
    for(Node* n : s){
        if(whichNode == i)
            return n;
        i++;
    }
    return nullptr;
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
        p->setActiveCL(q->getActiveCL());
        p->setActiveTP(q->getActiveTP());
        p->setNeedsCLUpdate(q->getNeedsCLUpdate());
        p->setNeedsTPUpdate(q->getNeedsTPUpdate());

        if(q->getAncestor() != nullptr){
            Node* ancestor = this->nodes[q->getAncestor()->getOffset()];
            double bl = t.getBranchLength(q, q->getAncestor());
            p->setAncestor(ancestor);
            setBranchLength(p, ancestor, bl);
        }
        else
            p->setAncestor(nullptr);

        p->removeAllNeighbors();
        std::vector<Node*>& qNeighbors = q->getNeighbors();
        for(Node* n : qNeighbors)
            p->addNeighbor(this->nodes[n->getOffset()]);
    }

    this->postOrderSeq.clear();
    for(int i = 0; i < t.postOrderSeq.size(); i++){
        Node* p = t.postOrderSeq[i];
        this->postOrderSeq.push_back(this->nodes[p->getOffset()]);
    }
}

void Tree::deleteAllNodes(){
    for (int i = 0; i < nodes.size(); i++)
        delete nodes[i];
    nodes.clear();
}

void Tree::flipAllCLs(){
    for(Node* n : nodes){
        n->flipCL();
    }
}

void Tree::flipAllTPs(){
    for(Node* n : nodes){
        n->flipTP();
    }
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

std::string Tree::getNewick() const{
    std::stringstream strm;
    writeNode(root, strm);
    strm << ";";
    return strm.str();
}

std::vector<Node*> Tree::getTips() {
    std::vector<Node*> out;
    out.reserve(numTaxa);

    for(Node* n : nodes){
        if(n->getIsTip())
            out.push_back(n);
    }

    return out;
}

int Tree::getTaxonIndex(std::string token, std::vector<std::string> taxaNames){

    for(int i = 0, n = taxaNames.size(); i < n; i++){
        if(taxaNames[i] == token)
            return i;
    }

    return -1;
}

void Tree::initPostOrder(void) {

    postOrderSeq.clear();
    passDown(root, root);
}

std::vector<std::string> Tree::parseNewickString(std::string newick){
    std::vector<std::string> tokens;
    std::string str = "";
    for(int i = 0; i < newick.length(); i++){
        char c = newick[i];
        if(c == '(' || c == ')' || c == ',' || c == ':' || c == ';'){
            if(str != ""){
                tokens.push_back(str);
                str = "";
            }

            tokens.push_back(std::string(1, c));
        }
        else if(c != ' '){
            str += std::string(1, c);
        }
    }

    return tokens;
}

void Tree::passDown(Node* p, Node* fromNode) {

    if(p == nullptr)
        return;
    
    std::vector<Node*>& pNeighbors = p->getNeighbors();

    for(Node* n : pNeighbors)
        {
        if(n != p->getAncestor())
            passDown(n, p);
        }

    if(p != fromNode)
        p->setAncestor(fromNode);

    postOrderSeq.push_back(p);
}

void Tree::print(std::string header) const{
    std::cout << header << std::endl;
    print();
}

void Tree::print(void) const{

    showNode(root, 0);
}

void Tree::removeBranchLength(Node* p1, Node* p2){
    std::pair<Node*, Node*> key;
    if(p1 < p2)
        key = std::make_pair(p1,p2);
    else
        key = std::make_pair(p2, p1);
    branchLengths.erase(key);
}

//Output nodes of the tree in a whitespace-indented format
void Tree::showNode(Node* p, int indent) const{

    if(p == nullptr)
        return;

    for(int i = 0; i < indent; i++)
        std::cout << " ";

    std::cout << p->getIndex() << " ( ";
    std::vector<Node*>& pNeighbors = p->getNeighbors();
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

double Tree::update(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    double updateVal = 0.0;

    if(rng.uniformRv() < 0.5)
        updateVal = updateBranchLength(&rng);
    else
        updateVal = updateNNI(&rng);

    return updateVal;
}

void Tree::updateAll(){
    for(Node* n : nodes){
        if(n->getIsTip() == false)
            n->setNeedsCLUpdate(true);
            n->setNeedsTPUpdate(true);
    }
}

double Tree::updateBranchLength(RandomVariable* rng){
    Node* p = nullptr;
    do{
        p = nodes[(int)(rng->uniformRv() * nodes.size())];
    }
    while(p == root);

    double currentV = getBranchLength(p, p->getAncestor());
    double tuning = std::log(4.0);
    double newV = currentV * exp(tuning * (rng->uniformRv() - 0.5));
    setBranchLength(p, p->getAncestor(), newV);
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

double Tree::updateLocal(RandomVariable* rng){
    return 0.0;
}


double Tree::updateNNI(RandomVariable* rng){
    Node* p = nullptr;
    do{
        p = nodes[(int)(rng->uniformRv() * nodes.size())];
    }
    while(p == root || p->getIsTip() == true);

    Node* a = p->getAncestor();

    Node* n1 = nullptr;
    std::vector<Node*> neighbors1 = p->getNeighbors();
    do{
        n1 = neighbors1[(int)(rng->uniformRv() * neighbors1.size())];
    }
    while(n1 == a);

    Node* n2 = nullptr;
    std::vector<Node*> neighbors2 = a->getNeighbors();
    do{
        n2 = neighbors2[(int)(rng->uniformRv() * neighbors2.size())];
    }
    while(n2 == p || n2 == a->getAncestor());

    

    //Add new neighbors and keep branch lengths
    double l1 = getBranchLength(p, n1);
    double l2 = getBranchLength(a, n2);
    n1->addNeighbor(a);
    a->addNeighbor(n1);
    n1->setAncestor(a);
    n2->addNeighbor(p);
    p->addNeighbor(n2);
    n2->setAncestor(p);
    setBranchLength(a, n1, l1);
    setBranchLength(p, n2, l2);

    //Remove old connections
    n1->removeNeighbor(p);
    p->removeNeighbor(n1);
    n2->removeNeighbor(a);
    a->removeNeighbor(n2);
    removeBranchLength(n1, p);
    removeBranchLength(n2, a);

    initPostOrder();

    return 0.0;
}


//For outputting a newick string
void Tree::writeNode(Node* p, std::stringstream& strm) const{
    if(p == nullptr)
        return;
    
    if(!p->getIsTip())
        strm << "(";
    else
        strm << p->getName();

    std::vector<Node*>& pDesc = p->getNeighbors();
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
