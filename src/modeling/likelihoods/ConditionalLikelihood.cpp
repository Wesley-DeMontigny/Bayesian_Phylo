#include "ConditionalLikelihood.hpp"
#include "core/Alignment.hpp"

ConditionalLikelihood::ConditionalLikelihood(Alignment* aln) : numNodes(aln->getNumTaxa() * 2 - 1) {
    numChar = aln->getNumChar();
    stateSpace = aln->getStateSpace();
    int width = numNodes*numChar*stateSpace;
    condLikelihoods[0] = new double[2 * width];
    condLikelihoods[1] = condLikelihoods[0] + (width);

    activeCLs = new int[numNodes];
    for(int i = 0; i < numNodes; i++)
        activeCLs[i] = 0;

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

ConditionalLikelihood::ConditionalLikelihood(int numTaxa, int s) : numNodes(numTaxa * 2 - 1), stateSpace(s) {
    int width = numNodes*numChar*stateSpace;
    condLikelihoods[0] = new double[2 * width];
    condLikelihoods[1] = condLikelihoods[0] + (width);

    activeCLs = new int[numNodes];
    for(int i = 0; i < numNodes; i++)
        activeCLs[i] = 0;

    for(int i = 0; i < width; i++){
        condLikelihoods[0][i] = 0.0;
        condLikelihoods[1][i] = 0.0;
    }
}

ConditionalLikelihood::~ConditionalLikelihood(){
    delete [] condLikelihoods[0];
    delete activeCLs;
}


double* ConditionalLikelihood::operator()(int n, int s){
    return condLikelihoods[s] + n*numChar*stateSpace;
}

double* ConditionalLikelihood::operator[](int n){
    return condLikelihoods[activeCLs[n]] + n*numChar*stateSpace;
}

void ConditionalLikelihood::flipCL(int n){
    activeCLs[n] ^= 1;
}