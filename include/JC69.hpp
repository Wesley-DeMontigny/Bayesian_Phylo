#ifndef JC69_Model
#define JC69_Model
#include "TreeModel.hpp"

class JC69 : public TreeModel{
    public:
        JC69();
        JC69(double l);
        void setLambda(double l) {lambda = l;}
        double getLambda() {return lambda;}
    protected:
        double lambda;
        std::vector<std::vector<double>> transitionProbability(double time);
        std::vector<double> stationaryDist;
};

#endif