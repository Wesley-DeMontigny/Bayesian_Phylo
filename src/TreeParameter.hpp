#ifndef TREE_PARAMETER_HPP
#define TREE_PARAMETER_HPP
#include "AbstractParameter.hpp"
#include "TreeObject.hpp"

class TreeParameter : public AbstractParameter{
    public:
        TreeParameter(void)=delete;
        TreeParameter(TreeObject* t);
        ~TreeParameter();
        void accept();
        TreeObject* getValue(){return trees[0];}
        void reject();
    private:
        TreeObject* trees[2];
};

#endif