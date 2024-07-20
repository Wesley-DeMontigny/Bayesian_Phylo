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

void TransitionProbability::set(int s, int n, double v){
    DoubleMatrix* P = probs[s][n];

    double expX = std::exp((-4.0/3.0) * v);
    double p0 = 0.25 + (0.75 * expX);
    double p1 = 0.25 - (0.25 * expX);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(i == j)
                (*P)(i, j) = p0;
            else
                (*P)(i, j) = p1;
        }
    }
}