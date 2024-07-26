#ifndef TREE_PARAMETER_HPP
#define TREE_PARAMETER_HPP
#include "modeling/ModelNode.hpp"
#include "TreeObject.hpp"
#include <string>

class TreeParameter : public ModelNode{
    public:
        TreeParameter(void)=delete;
        template<typename... Args> TreeParameter(Args&&... args);
        ~TreeParameter();
        TreeObject* getTree(){return trees[0];}
        std::vector<double*> getBranchLengths();
        void accept();
        void reject();
        void regenerate();
        std::string writeValue();
    private:
        TreeObject* trees[2];
};

template<typename... Args>
TreeParameter::TreeParameter(Args&&... args)
    : trees{new TreeObject(std::forward<Args>(args)...), nullptr} {
    trees[1] = new TreeObject(*trees[0]);
    dirty();
}

#endif