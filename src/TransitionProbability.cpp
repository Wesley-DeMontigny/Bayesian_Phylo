#include "TransitionProbability.hpp"
#include "DoubleMatrix.hpp"
#include <cmath>

TransitionProbability::TransitionProbability(int nn, int s) : numNodes(nn), stateSpace(s) {
    probs[0] = new DoubleMatrix*[2*numNodes];
    probs[1] = probs[0] + numNodes;

    for(int i = 0; i < numNodes; i++){
        probs[0][i] = new DoubleMatrix(stateSpace);
        probs[1][i] = new DoubleMatrix(stateSpace);
    }

    stationaryFreqs.resize(stateSpace);
    for(int i = 0; i < stateSpace; i++){
        stationaryFreqs[i] = 1.0 / (double)(stateSpace);
    }
}

TransitionProbability::~TransitionProbability(){
    for(int i = 0; i < numNodes; i++){
        delete probs[0][i];
        delete probs[1][i];
    }

    delete [] probs[0];
}

DoubleMatrix* TransitionProbability::operator()(int s, int n){
    return probs[s][n];
}

void TransitionProbability::set(int s, int n, DoubleMatrix m){
    *probs[s][n] = m;
}