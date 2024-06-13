#ifndef tree_likelihood
#define tree_likelihood
#include "EvolutionaryModel.hpp"

class TreeLikelihood{
    public:
        TreeLikelihood(void) = delete;
        static double FelPrune(EvolutionaryModel* model);
};

#endif