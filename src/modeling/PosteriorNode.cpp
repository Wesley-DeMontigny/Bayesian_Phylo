#include "PosteriorNode.hpp"

PosteriorNode::PosteriorNode(LikelihoodNode* lN, PriorNode* pN) : likelihood(lN), prior(pN){
    this->addChild(lN);
    this->addChild(pN);
    this->dirty();
}

void PosteriorNode::accept() {
    oldLnPosterior = currentLnPosterior;
    for(ModelNode* c : this->getChildren()){
        c->accept();
        c->clean();
    }
}

void PosteriorNode::reject() {
    currentLnPosterior = oldLnPosterior;
    for(ModelNode* c : this->getChildren()){
        c->reject();
        c->clean();
    }
}

void PosteriorNode::regenerate(){
    for(ModelNode* c : this->getChildren()){
        if(c->isDirty())
            this->dirty();
        c->regenerate();
    }

    if(this->isDirty()){
        currentLnPosterior = likelihood->lnLikelihood() + prior->lnPrior();
    }
}