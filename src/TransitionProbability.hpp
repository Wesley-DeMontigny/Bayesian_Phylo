#ifndef TRANSITION_PROBABILITY_HPP
#define TRANSITION_PROBABILITY_HPP
#include <vector>

class DoubleMatrix;

class TransitionProbability{
    public:
        TransitionProbability(void) = delete;
        TransitionProbability(int nn);
        ~TransitionProbability();
        DoubleMatrix* operator()(int s, int n);
        std::vector<double>& getStationaryFreq(){return stationaryFreqs;}
        void set(int space, int nodeIdx, double v);
    private:
        int numNodes;
        DoubleMatrix** probs[2];
        std::vector<double> stationaryFreqs;
};

#endif