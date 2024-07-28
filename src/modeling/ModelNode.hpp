#ifndef MODEL_NODE_HPP
#define MODEL_NODE_HPP
#include <vector>
#include <string>

class ModelNode {
    public:
        std::vector<ModelNode*> getChildren() {return children;}
        virtual void accept()=0;
        virtual void reject()=0;
        virtual void regenerate()=0;
        virtual std::string writeValue()=0;
        void clean() {dirtyFlag = false;}
        void dirty() {dirtyFlag = true;}
        bool isDirty() {return dirtyFlag;}
    protected:
        void addChild(ModelNode* c) {children.push_back(c);}
    private:
        std::vector<ModelNode*> children;
        bool dirtyFlag;
};

#endif