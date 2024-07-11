#ifndef PHYLO_CTMC_HPP
#define PHYLO_CTMC_HPP
#include "AbstractDistribution.hpp"
#include "Tree.hpp"

class ConditionalLikelihood;
class TransitionProbability;
class Alignment;
class RandomVariable;

class PhyloCTMC : public AbstractDistribution{
    public:
        PhyloCTMC(void) = delete;
        PhyloCTMC(Alignment* a);
        PhyloCTMC(Alignment* a, Tree* t);
        ~PhyloCTMC();
        void accept();
        Tree getActiveTree(){return *tree[0];}
        double lnLikelihood();
        double lnPrior();
        void reject();
        double update();
    private:
        Alignment* aln;
        ConditionalLikelihood* condL;
        RandomVariable* rng;
        TransitionProbability* transProb;
        Tree* tree[2];
};

#endif