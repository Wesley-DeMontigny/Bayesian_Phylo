#include "TransitionProbability.hpp"
#include "DoubleMatrix.hpp"
#include <cmath>

TransitionProbability::TransitionProbability(int nn) : numNodes(nn) {
    probs[0] = new DoubleMatrix*[2*numNodes];
    probs[1] = probs[0] + numNodes;

    for(int i = 0; i < numNodes; i++){
        probs[0][i] = new DoubleMatrix(numNodes);
        probs[1][i] = new DoubleMatrix(numNodes);
    }

    stationaryFreqs.resize(4);
    for(int i = 0; i < 4; i++){
        stationaryFreqs[i] = 0.25;
    }
}

TransitionProbability::~TransitionProbability(){
    for(int i = 0; i < numNodes; i++){
        delete probs[0][i];
        delete probs[1][i];
    }

    delete [] probs[0];
}

DoubleMatrix* TransitionProbability::operator()(int n, int s){
    return probs[s][n];
}

void TransitionProbability::set(int space, int nodeIdx, double v){
    DoubleMatrix* P = probs[space][nodeIdx];

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