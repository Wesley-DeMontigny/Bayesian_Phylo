#ifndef TREE_PARAMETER_HPP
#define TREE_PARAMETER_HPP
#include "AbstractParameter.hpp"
#include "TreeObject.hpp"
#include "AbstractPrior.hpp"
#include "AbstractLikelihood.hpp"
#include "AbstractParameter.hpp"
#include <string>

class TreeParameter : public AbstractParameter{
    public:
        TreeParameter(void)=delete;
        template<typename... Args> TreeParameter(Args&&... args);
        ~TreeParameter();
        TreeObject* getTree(){return trees[0];}
        std::vector<double*> getBranchLengths();
        void accept();
        void reject();
        void setPrior(AbstractPrior* p) {prior=p;}
        void setLikelihood(AbstractLikelihood* l) {likelihood = l;}
        void regenerate();
        std::string writeValue();
    protected:
        AbstractPrior* prior;
        AbstractLikelihood* likelihood;
    private:
        TreeObject* trees[2];
};

template<typename... Args>
TreeParameter::TreeParameter(Args&&... args)
    : trees{new TreeObject(std::forward<Args>(args)...), nullptr}, prior(nullptr), likelihood(nullptr) {
    trees[1] = new TreeObject(*trees[0]);
}

#endif