#ifndef PHYLO_CTMC_HPP
#define PHYLO_CTMC_HPP
#include "modeling/likelihoods/LikelihoodNode.hpp"
#include "modeling/parameters/trees/TreeParameter.hpp"

class ConditionalLikelihood;
class TransitionProbability;
class Alignment;
class RandomVariable;
class RateMatrix;

class PhyloCTMC : public LikelihoodNode{
    public:
        PhyloCTMC(void) = delete;
        PhyloCTMC(Alignment* a, TreeParameter* t, RateMatrix* m);
        ~PhyloCTMC();
        double lnLikelihood() {return currentLikelihood;}
        void regenerate();
        void accept();
        void reject();
        TransitionProbability* getTransitionProbability() {return transProb;}
        ConditionalLikelihood* getConditionalLikelihood() {return condL;}
        std::string writeValue() {return std::to_string(currentLikelihood);}
    protected:
        double oldLikelihood;
        double currentLikelihood;
    private:
        int stateSpace;
        RateMatrix* rateMatrix;
        Alignment* aln;
        ConditionalLikelihood* condL;
        TransitionProbability* transProb;
        TreeParameter* tree;
};

#endif