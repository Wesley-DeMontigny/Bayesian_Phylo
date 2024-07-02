#ifndef evolutionary_model
#define evolutionary_model
#include "Alignment.hpp"
#include "Tree.hpp"
#include "ConditionalLikelihood.hpp"
#include "DoubleMatrix.hpp"

class EvolutionaryModel{
    public:
        EvolutionaryModel(void) = delete;
        EvolutionaryModel(Tree* t, Alignment* a, double* sD);
        ~EvolutionaryModel();
        Alignment* getAlignment() {return aln;}
        Tree* getTree() {return tree;}
        double* getSationaryDistribution() const {return stationaryDist;}
        void setStationaryDist(double* sD) {stationaryDist =  sD;}
        std::map<double, DoubleMatrix> getTransitionProbabilityMap() const {return transitionProbabilityMap;}
        ConditionalLikelihood* getConditionalLikelihood() {return condL;}
        void initializeTransitionProbabilityMap();
    private:
        virtual DoubleMatrix P(double time) = 0;
        std::map<double, DoubleMatrix> transitionProbabilityMap;
        ConditionalLikelihood* condL;
        Tree* tree;
        Alignment* aln;
        double* stationaryDist;
};

#endif