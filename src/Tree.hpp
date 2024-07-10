#ifndef TREE_HPP
#define TREE_HPP
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include "AbstractParameter.hpp"

class Node;
class Alignment;
class RandomVariable;

class Tree : public AbstractParameter {

    public:
                            Tree(void) = delete;
                            Tree(RandomVariable* rng, int nt);
                            Tree(RandomVariable* rng, Alignment* aln);
                            Tree(const Tree& t);
                            Tree(RandomVariable* rng, double lambda, double mu, double duration);
                            Tree(Alignment* aln, std::string method);
                            Tree(std::string newick, std::vector<std::string> taxaNames);
                           ~Tree(void);
        Tree&               operator=(const Tree& rhs);
        void                flipAllTPs();
        void                flipAllCLs();
        void                setBranchLength(Node* p1, Node* p2, double length);
        double              getBranchLength(Node* p1, Node* p2) const;
        std::string         getNewick() const;
        std::vector<Node*>& getPostOrderSeq() {return postOrderSeq;}
        Node*               getRoot() {return root;}
        std::vector<Node*>  getTips();
        void                print(void) const;
        void                print(std::string header) const;
        void                removeBranchLength(Node* p1, Node* p2);
        double              update();
        void                updateAll();
        
    private:
        Node*               addNode(void);
        std::map<std::pair<Node*, Node*>, double> branchLengths;
        Node*               chooseNodeFromSet(std::set<Node*>& s, RandomVariable* rng);
        void                clone(const Tree& t);
        void                deleteAllNodes();
        int                 getTaxonIndex(std::string token, std::vector<std::string> taxaNames);
        void                initPostOrder(void);
        std::vector<Node*>  nodes;
        int                 numTaxa;
        std::vector<std::string> parseNewickString(std::string newick);
        void                passDown(Node* p, Node* fromNode);
        std::vector<Node*>  postOrderSeq;
        Node*               root;
        void                showNode(Node* p, int indent) const;
        double              updateBranchLength(RandomVariable* rng);
        double              updateLocal(RandomVariable* rng);
        double              updateNNI(RandomVariable* rng);
        void                writeNode(Node* p, std::stringstream& strm) const;
};

#endif
