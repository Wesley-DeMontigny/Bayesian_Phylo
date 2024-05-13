#ifndef node_class
#define node_class
#include <set>
#include <string>

class Node{
    public:
        Node();
        Node* getAncestor() {return ancestor;}
        void setAncestor(Node* a) {ancestor = a;}
        std::set<Node*>& getNeighbors() {return neighbors;}
        void addNeighbor(Node* n) {neighbors.insert(n);}
        void removeNeighbor(Node* n) {neighbors.erase(n);}
        void removeAllNeighbors() {neighbors.clear();}
        int getIndex() {return index;}
        void setIndex(int i) {index = i;}
        bool getIsTip() {return isTip;}
        void setIsTip(bool t) {isTip = t;}
        std::string getName() {return name;}
        void setName(std::string s) {name = s;}
    private:
        Node* ancestor;
        std::set<Node*> neighbors;
        int index;
        bool isTip;
        std::string name;
};

#endif