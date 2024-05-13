#ifndef tree_class
#define tree_class
#include <vector>

class Node;
class RandomVariable;

class Tree {
    public:
        Tree(void) = delete; // Impossible to make a tree from the default constructor
        Tree(RandomVariable* rng, int nt);
        ~Tree();
        void print();
    private:
        int numTaxa;
        Node* root;
        std::vector<Node*> nodes;
        std::vector<Node*> postOrderSeq;
        Node* addNode();

        void initPostOrder();
        void passDown(Node* p, Node* fromNode);
        void showNode(Node* p, int indent);
};

#endif