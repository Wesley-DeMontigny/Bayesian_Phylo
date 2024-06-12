#include "ConditionalLikelihood.hpp"

ConditionalLikelihood::ConditionalLikelihood(Tree t, Alignment aln) : numNodes(t.getPostOrderSeq().size()), rootIndex(t.getRoot()->getIndex()) {
    //Indexing is (node_index*4 + state) + (site_index * num_nodes * 4)
    condLikelihoods.reserve(numNodes * aln.getNumChar() * 4);
    condLikelihoods.resize(numNodes * aln.getNumChar() * 4);

    //Iterate over all character sites
    for(int i = 0; i < aln.getNumChar(); i++){
        //Iterate over all tips
        for(Node* n : t.getTips()){
            int state = aln.getMatrix()[n->getAlignmentIndex()][i];

            int base_index = (n->getIndex() * 4) + (numNodes * 4 * i);
            
            double stateCount = 0;
            for(int j = 0; j < 4; j++) {
                condLikelihoods[base_index + j] = (state >> j & 1);
                stateCount += condLikelihoods[base_index + j];
            }

            for(int k = 0; k < 4; k++)
                condLikelihoods[base_index + k] = condLikelihoods[base_index + k] / stateCount;
        }

    }
}

ConditionalLikelihood::~ConditionalLikelihood(){
}

double* ConditionalLikelihood::getRootConditionalLikelihood(int site, int characterState){
    return &condLikelihoods[(rootIndex * 4 + characterState) + (numNodes * 4 * site)];
}

double* ConditionalLikelihood::getCondLikelihood(int nodeIndex, int site, int characterState){
    return &condLikelihoods[(nodeIndex * 4 + characterState) + (numNodes * 4 * site)];
}