#ifndef node_class
#define node_class
#include <set>
#include <string>

class Node{
    public:
        Node();
        Node* getAncestor() const {return ancestor;}
        void setAncestor(Node* a) {ancestor = a;}
        std::set<Node*>& getNeighbors() {return neighbors;}
        void addNeighbor(Node* n) {neighbors.insert(n);}
        void removeNeighbor(Node* n) {neighbors.erase(n);}
        void removeAllNeighbors() {neighbors.clear();}
        int getOffset() const {return offset;} 
        void setOffset(int o){offset = o;}
        int getIndex() const {return index;}
        void setIndex(int i) {index = i;}
        bool getIsTip() const {return isTip;}
        void setIsTip(bool t) {isTip = t;}
        std::string getName() const {return name;}
        void setName(std::string s) {name = s;}
    private:
        Node* ancestor;
        std::set<Node*> neighbors;
        int index;
        bool isTip;
        std::string name;
        int offset;
};

#endif