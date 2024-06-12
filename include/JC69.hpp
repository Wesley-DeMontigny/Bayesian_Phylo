#ifndef JC69_Model
#define JC69_Model
#include "TreeModel.hpp"

class JC69 : public TreeModel{
    public:
        JC69(void);
    protected:
        std::vector<std::vector<double>> transitionProbability(double time);
        std::vector<double> stationaryDist;
};

#endif