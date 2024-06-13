#ifndef evolutionary_model
#define evolutionary_model
#include "Alignment.hpp"
#include "Tree.hpp"
#include "ConditionalLikelihood.hpp"

class EvolutionaryModel{
    public:
        EvolutionaryModel(void) = delete;
        EvolutionaryModel(Tree* t, Alignment* a, std::vector<double> sD);
        Alignment* getAlignment() {return aln;}
        Tree* getTree() {return tree;}
        std::vector<double> getSationaryDistribution() const {return stationaryDist;}
        void setStationaryDist(std::vector<double> sD) {stationaryDist =  sD;}
        std::map<double, std::vector<std::vector<double>>> getTransitionProbabilityMap() const {return transitionProbabilityMap;}
        ConditionalLikelihood* getConditionalLikelihood() {return condL;}
        void initializeTransitionProbabilityMap();
    private:
        virtual std::vector<std::vector<double>> P(double time) = 0;
        std::map<double, std::vector<std::vector<double>>> transitionProbabilityMap;
        ConditionalLikelihood* condL;
        Tree* tree;
        Alignment* aln;
        std::vector<double> stationaryDist;
};

#endif