#include "ConditionalLikelihood.hpp"

ConditionalLikelihood::ConditionalLikelihood(Tree* t, Alignment* aln) : numNodes(t->getPostOrderSeq().size()), rootIndex(t->getRoot()->getIndex()), active(0) {
    numChar = aln->getNumChar();
    condLikelihoods[0] = new double[2*numNodes*numChar*4];
    condLikelihoods[1] = condLikelihoods[0] + (numNodes*numChar*4);

    for(int i = 0; i < numNodes*numChar*4; i++){
        condLikelihoods[0][i] = 0.0;
        condLikelihoods[1][i] = 0.0;
    }

    for(Node* n : t->getTips()){
        double* p = (*this)(n->getIndex(), 0);
        for(int i = 0; i < numChar; i++){
            unsigned state = aln->getMatrix()[n->getIndex()][i];

            unsigned mask = 1;

            for(int j = 0; j < 4; j++) {
                if((mask & state) != 0)
                    *p = 1.0;
                mask >>= 1;
                p++;
            }
        }

    }
}

ConditionalLikelihood::~ConditionalLikelihood(){
    delete [] condLikelihoods[0];
}


double* ConditionalLikelihood::operator()(int n, int s){
    return condLikelihoods[s] + n*numChar*4;
}


int ConditionalLikelihood::swapActiveState(){
    activeState = activeState ^ 1; // Bitwise XOR Swap
    return activeState;
}