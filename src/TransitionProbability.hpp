#ifndef TRANSITION_PROBABILITY_HPP
#define TRANSITION_PROBABILITY_HPP
#include <vector>

class DoubleMatrix;

class TransitionProbability{
    public:
        TransitionProbability(void) = delete;
        TransitionProbability(int nn, int s);
        ~TransitionProbability();
        DoubleMatrix* operator()(int s, int n);
        std::vector<double>& getStationaryFreq(){return stationaryFreqs;}
        int getStateSpace() {return stateSpace;}
        void set(int s, int n, DoubleMatrix m);
    private:
        int numNodes;
        int stateSpace;
        DoubleMatrix** probs[2];
        std::vector<double> stationaryFreqs;
};

#endif