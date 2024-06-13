#ifndef JC69_Model
#define JC69_Model
#include "EvolutionaryModel.hpp"

class JC69 : public EvolutionaryModel {
    public:
        JC69(Tree* t, Alignment* a, std::vector<double> sD);
    protected:
        std::vector<std::vector<double>> P(double time);
};

#endif