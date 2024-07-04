#ifndef conditional_likelihood
#define conditional_likelihood
#include "Alignment.hpp"
#include "Node.hpp"
#include "Tree.hpp"

class ConditionalLikelihood{
    public:
        ConditionalLikelihood(void) = delete;
        ConditionalLikelihood(Alignment* aln);
        ~ConditionalLikelihood();
        double* operator()(int n, int s);
    private:
        double* condLikelihoods[2];
        int numChar;
        int numNodes;
};

#endif