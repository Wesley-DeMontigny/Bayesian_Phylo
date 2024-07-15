#ifndef TREE_PARAMETER_HPP
#define TREE_PARAMETER_HPP
#include "AbstractParameter.hpp"
#include "TreeObject.hpp"

class TreeParameter : public AbstractParameter{
    public:
        TreeParameter(void)=delete;
        template<typename... Args> TreeParameter(Args&&... args);
        ~TreeParameter();
        TreeObject* getTree(){return trees[0];}
        std::vector<double*> getBranchLengths();
        void accept();
        void reject();
    private:
        TreeObject* trees[2];
};

template<typename... Args>
TreeParameter::TreeParameter(Args&&... args)
    : trees{new TreeObject(std::forward<Args>(args)...), nullptr} {
    trees[1] = new TreeObject(*trees[0]);
}

#endif