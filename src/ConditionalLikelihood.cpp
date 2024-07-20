#include "ConditionalLikelihood.hpp"
#include "Alignment.hpp"

ConditionalLikelihood::ConditionalLikelihood(Alignment* aln) : numNodes(aln->getNumTaxa() * 2 - 1) {
    numChar = aln->getNumChar();
    stateSpace = aln->getStateSpace();
    int width = numNodes*numChar*stateSpace;
    condLikelihoods[0] = new double[2 * width];
    condLikelihoods[1] = condLikelihoods[0] + (width);

    for(int i = 0; i < width; i++){
        condLikelihoods[0][i] = 0.0;
        condLikelihoods[1][i] = 0.0;
    }

    for(int index = 0; index < aln->getNumTaxa(); index++){
        double* p = (*this)(index, 0);
        for(int i = 0; i < numChar; i++){
            unsigned state = aln->getMatrix()[index][i];

            unsigned mask = 1;

            for(int j = 0; j < stateSpace; j++) {
                if((mask & state) != 0)
                    *p = 1.0;
                mask <<= 1;
                p++;
            }
        }

    }
}

ConditionalLikelihood::~ConditionalLikelihood(){
    delete [] condLikelihoods[0];
}


double* ConditionalLikelihood::operator()(int n, int s){
    return condLikelihoods[s] + n*numChar*stateSpace;
}