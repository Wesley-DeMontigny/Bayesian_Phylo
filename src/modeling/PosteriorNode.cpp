#include "PosteriorNode.hpp"
#include <iostream>

PosteriorNode::PosteriorNode(LikelihoodNode* lN, PriorNode* pN) : likelihood(lN), prior(pN), currentLnPosterior(0.0), oldLnPosterior(0.0) {
    this->addChild(lN);
    this->addChild(pN);
    this->dirty();
}

void PosteriorNode::accept() {
    for(ModelNode* c : this->getChildren()){
        c->accept();
        c->clean();
    }

    oldLnPosterior = currentLnPosterior;
}

void PosteriorNode::reject() {
    for(ModelNode* c : this->getChildren()){
        c->reject();
        c->clean();
    }

    currentLnPosterior = oldLnPosterior;
}

void PosteriorNode::regenerate(){
    for(ModelNode* c : this->getChildren()){
        c->regenerate();
        if(c->isDirty())
            this->dirty();
    }

    if(this->isDirty()){
        currentLnPosterior = likelihood->lnLikelihood() + prior->lnPrior();
    }
}