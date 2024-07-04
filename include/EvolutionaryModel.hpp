#ifndef evolutionary_model
#define evolutionary_model

class ConditionalLikelihood;
class TransitionProbability;
class Tree;
class Alignment;
class RandomVariable;

class EvolutionaryModel{
    public:
        EvolutionaryModel(void) = delete;
        EvolutionaryModel(Alignment* a, RandomVariable* rng);
        ~EvolutionaryModel();
        double lnLikelihood();
    private:
        int activeTree;
        Alignment* aln;
        ConditionalLikelihood* condL;
        RandomVariable* rng;
        TransitionProbability* transProb;
        Tree* tree[2];
};

#endif