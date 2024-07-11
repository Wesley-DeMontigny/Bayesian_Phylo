#ifndef TREE_OBJECT_HPP
#define TREE_OBJECT_HPP
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

class Node;
class Alignment;
class RandomVariable;

class TreeObject {

    public:
                            TreeObject(void) = delete;
                            TreeObject(int nt);
                            TreeObject(Alignment* aln);
                            TreeObject(const TreeObject& t);
                            TreeObject(double lambda, double mu, double duration);
                            TreeObject(std::string newick, std::vector<std::string> taxaNames);
                           ~TreeObject(void);
        TreeObject&               operator=(const TreeObject& rhs);
        void                flipAllTPs();
        void                flipAllCLs();
        double              getBranchLength(Node* p1, Node* p2) const;
        std::string         getNewick() const;
        int                 getNumTaxa(){return numTaxa;}
        std::vector<Node*>& getPostOrderSeq() {return postOrderSeq;}
        Node*               getRoot() {return root;}
        std::vector<Node*>  getTips();
        void                initPostOrder(void);
        void                print(void) const;
        void                print(std::string header) const;
        void                removeBranchLength(Node* p1, Node* p2);
        void                setBranchLength(Node* p1, Node* p2, double length);
        void                updateAll();
        
    private:
        Node*               addNode(void);
        std::map<std::pair<Node*, Node*>, double> branchLengths;
        void                clone(const TreeObject& t);
        void                deleteAllNodes();
        int                 getTaxonIndex(std::string token, std::vector<std::string> taxaNames);
        std::vector<Node*>  nodes;
        int                 numTaxa;
        std::vector<std::string> parseNewickString(std::string newick);
        void                passDown(Node* p, Node* fromNode);
        std::vector<Node*>  postOrderSeq;
        Node*               root;
        void                showNode(Node* p, int indent) const;
        void                writeNode(Node* p, std::stringstream& strm) const;
};

#endif
