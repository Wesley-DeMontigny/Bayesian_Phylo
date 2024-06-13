#include "EvolutionaryModel.hpp"

EvolutionaryModel::EvolutionaryModel(Tree* t, Alignment* a,
                                     std::vector<double> sD) :
                                     tree(t), aln(a), stationaryDist(sD) {
    condL = new ConditionalLikelihood(t, aln);
}

void EvolutionaryModel::initializeTransitionProbabilityMap() {
    //I set branch lengths as the key to the transition probability map. This could save space in the case that there
    //are multiple branches with the same length.
    transitionProbabilityMap.clear();
    for(Node* n : tree->getPostOrderSeq()){
        if (n->getAncestor() != nullptr){
            double length = tree->getBranchLength(n, n->getAncestor());
            transitionProbabilityMap.insert(
                {length, P(length)}
                );
        }
    }
}