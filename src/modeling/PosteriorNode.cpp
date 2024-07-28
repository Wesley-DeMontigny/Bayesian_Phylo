#include "PosteriorNode.hpp"
#include <iostream>

PosteriorNode::PosteriorNode(LikelihoodNode* lN, PriorNode* pN) : likelihood(lN), prior(pN), currentLnPosterior(0.0), oldLnPosterior(0.0) {
    this->dirty();
}

void PosteriorNode::accept() {
    oldLnPosterior = currentLnPosterior;

    likelihood->accept();
    likelihood->clean();
    prior->accept();
    prior->clean();
}

void PosteriorNode::reject() {
    currentLnPosterior = oldLnPosterior;

    likelihood->reject();
    likelihood->clean();
    prior->reject();
    prior->clean();
}

void PosteriorNode::regenerate(){
    likelihood->regenerate();
    prior->regenerate();
    if(prior->isDirty() || likelihood->isDirty())
        this->dirty();

    if(this->isDirty())
        currentLnPosterior = likelihood->lnLikelihood() + prior->lnPrior();
}