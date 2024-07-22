#ifndef PHYLO_CTMC_HPP
#define PHYLO_CTMC_HPP
#include "AbstractLikelihood.hpp"
#include "TreeParameter.hpp"

class ConditionalLikelihood;
class TransitionProbability;
class Alignment;
class RandomVariable;
class RateMatrix;

class PhyloCTMC : public AbstractLikelihood{
    public:
        PhyloCTMC(void) = delete;
        PhyloCTMC(Alignment* a, TreeParameter* t, RateMatrix* m);
        ~PhyloCTMC();
        double lnLikelihood() {return currentLikelihood;}
        void regenerateLikelihood();
        void acceptLikelihood() {oldLikelihood = currentLikelihood;}
        void rejectLikelihood() {currentLikelihood = oldLikelihood;}
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