#include "PosteriorNode.hpp"

PosteriorNode::PosteriorNode(LikelihoodNode* lN, PriorNode* pN) : likelihood(lN), prior(pN){
    addChild(lN);
    addChild(pN);
}

void PosteriorNode::regenerate(){
    currentPosterior = likelihood->lnLikelihood() + prior->lnPrior();
}