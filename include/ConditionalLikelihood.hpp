#ifndef conditional_likelihood
#define conditional_likelihood
#include "Alignment.hpp"
#include "Node.hpp"
#include "Tree.hpp"

class ConditionalLikelihood{
    public:
        ConditionalLikelihood(void) = delete;
        ConditionalLikelihood(Tree* t, Alignment* aln);
        ~ConditionalLikelihood();
        int getRootIndex(){return rootIndex;}
        double* operator()(int n, int s);
        int getActiveState(){return activeState;};
        int swapActiveState();
    private:
        double* condLikelihoods[2];
        int numChar;
        int numNodes;
        int rootIndex;
        int activeState;
};

#endif