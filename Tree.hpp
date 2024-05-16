#ifndef tree_class
#define tree_class
#include <vector>

class Node;
class RandomVariable;

class Tree {

    public:
                            Tree(void) = delete; // Impossible to make a tree from the default constructor
                            Tree(RandomVariable* rng, int nt);
                           ~Tree(void);
        void                print(void);
        
    private:
        Node*               addNode(void);
        void                initPostOrder(void);
        void                passDown(Node* p, Node* fromNode);
        void                showNode(Node* p, int indent);
        int                 numTaxa;
        Node*               root;
        std::vector<Node*>  nodes;
        std::vector<Node*>  postOrderSeq;
};

#endif
