#ifndef conditional_likelihood
#define conditional_likelihood
#include "Alignment.hpp"
#include "Node.hpp"
#include "Tree.hpp"

class ConditionalLikelihood{
    public:
        ConditionalLikelihood(void) = delete;
        ConditionalLikelihood(Tree t, Alignment aln);
        ~ConditionalLikelihood();
        double* getCondLikelihood(int nodeIndex, int site, int characterState);
        double* getRootConditionalLikelihood(int site, int characterState);
    private:
        std::vector<double> condLikelihoods;
        const int numNodes;
        const int rootIndex;
};

#endif