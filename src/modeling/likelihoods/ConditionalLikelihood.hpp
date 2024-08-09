#ifndef CONDITIONAL_LIKELIHOOD_HPP
#define CONDITIONAL_LIKELIHOOD_HPP

class Alignment;
class Node;

class ConditionalLikelihood{
    public:
        ConditionalLikelihood(void) = delete;
        ConditionalLikelihood(Alignment* aln);
        ConditionalLikelihood(int numTaxa, int s);
        ~ConditionalLikelihood();
        double* operator()(int n, int s);
        double* operator[](int n);
        void flipCL(int n);
    private:
        double* condLikelihoods[2];
        int* activeCLs;
        int numChar;
        int numNodes;
        int stateSpace;
};

#endif