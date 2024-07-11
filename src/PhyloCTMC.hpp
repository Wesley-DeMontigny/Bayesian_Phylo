#ifndef PHYLO_CTMC_HPP
#define PHYLO_CTMC_HPP
#include "AbstractDistribution.hpp"
#include "TreeParameter.hpp"

class ConditionalLikelihood;
class TransitionProbability;
class Alignment;
class RandomVariable;

class PhyloCTMC : public AbstractDistribution{
    public:
        PhyloCTMC(void) = delete;
        PhyloCTMC(Alignment* a, TreeParameter* t);
        ~PhyloCTMC();
        double lnLikelihood();
    private:
        Alignment* aln;
        ConditionalLikelihood* condL;
        TransitionProbability* transProb;
        TreeParameter* tree;
};

#endif