#ifndef POSTERIOR_NODE_HPP
#define POSTERIOR_NODE_HPP
#include "ModelNode.hpp"
#include "modeling/likelihoods/LikelihoodNode.hpp"
#include "modeling/priors/PriorNode.hpp"

class PosteriorNode : public ModelNode {
    public:
        PosteriorNode(void)=delete;
        PosteriorNode(LikelihoodNode* lN, PriorNode* pN);//May want to modify this to accept multiple priors and likelihoods
        void accept() {oldPosterior = currentPosterior;}
        void reject() {currentPosterior = oldPosterior;}
        void regenerate();
        double lnPosterior() {return currentPosterior;}
        std::string writeValue() {return std::to_string(currentPosterior);}
    private:
        LikelihoodNode* likelihood;
        PriorNode* prior;
        double currentPosterior;
        double oldPosterior;
};

#endif