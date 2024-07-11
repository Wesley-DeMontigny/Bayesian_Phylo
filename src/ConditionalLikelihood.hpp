#ifndef CONDITIONAL_LIKELIHOOD_HPP
#define CONDITIONAL_LIKELIHOOD_HPP

class Alignment;
class Node;

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