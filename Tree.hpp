#ifndef tree_class
#define tree_class
#include <vector>
#include <string>
#include <sstream>
#include <map>

class Node;
class RandomVariable;

class Tree {

    public:
                            Tree(void) = delete; // Impossible to make a tree from the default constructor
                            Tree(RandomVariable* rng, int nt);
                            Tree(const Tree& t);
                           ~Tree(void);
        Tree&               operator=(const Tree& rhs);
        void                print(void) const;
        void                print(std::string header) const;
        void                setBranchLength(Node* p1, Node* p2, double length);
        double              getBranchLength(Node* p1, Node* p2) const;
        std::string         getNewick() const;
        
    private:
        Node*               addNode(void);
        void                initPostOrder(void);
        void                passDown(Node* p, Node* fromNode);
        void                showNode(Node* p, int indent) const;
        void                clone(const Tree& t);
        void                deleteAllNodes();
        void                writeNode(Node* p, std::stringstream& strm) const;
        int                 numTaxa;
        std::map<std::pair<Node*, Node*>, double> branchLengths;
        Node*               root;
        std::vector<Node*>  nodes;
        std::vector<Node*>  postOrderSeq;
};

#endif
