#include "TreeLikelihood.hpp"
#include "Msg.hpp"
#include "Alignment.hpp"
#include "Tree.hpp"
#include "ConditionalLikelihood.hpp"
#include <cmath>

double TreeLikelihood::FelPrune(EvolutionaryModel* model){
    
    Tree* t = model->getTree();
    Alignment* aln = model->getAlignment();
    ConditionalLikelihood* condL = model->getConditionalLikelihood();
    int activeState = condL->getActiveState();
    double* stationaryDist = model->getSationaryDistribution();
    std::map<double, DoubleMatrix> transitionProbMap = model->getTransitionProbabilityMap();

    if(aln->getDataType() != NxsCharactersBlock::dna && aln->getDataType() != NxsCharactersBlock::rna && aln->getDataType() != NxsCharactersBlock::nucleotide)
        Msg::error("Non-nucleotide data is currently not implemented");

    std::vector<Node*> nodes = t->getPostOrderSeq();

    double* siteLogLikelihoods = new double[aln->getNumChar()];
    for(int i = 0; i < aln->getNumChar(); i++)
        siteLogLikelihoods[i] = 0.0;

    //Iterate site by site to get site log likelihoods
    for(int i = 0; i < aln->getNumChar(); i++){

        for(Node* n : nodes){
            //See p. 104 of Molecular Evolution: A Statistical Approach (Yang 2014)
            if(!n->getIsTip()){
                std::set<Node*> neighbors = n->getNeighbors();
                int nIndex = n->getIndex();

                //Iterate over ancestral states
                for(int j = 0; j < 4; j++){
                    double stateL = 1;
                    //Iterate over children
                    for(Node* neighbor : neighbors){
                        double childL = 0.0;
                        if(neighbor != n->getAncestor()){
                            //Get the transition probability from the map
                            double length = t->getBranchLength(n, neighbor);
                            auto itTP = transitionProbMap.find(length);
                            DoubleMatrix P = itTP->second;

                            int neighborIndex = neighbor->getIndex();

                            //Iterate over children states
                            for(int k = 0; k < 4; k++)
                                childL += (P(j, k) * (*(*condL)(neighborIndex, activeState) + i*4 + k));

                            stateL *= childL;
                        }
                    }
                    *((*condL)(nIndex, 0) + i*4 + j) = stateL;
                }
            }
        }

        //Final likelihood is summing over the product of the likelihoods up to the root and the stationary distribution.
        double siteLogL = 0.0;
        for(int j = 0; j < 4; j++)
            siteLogL += (*((*condL)(condL->getRootIndex(), activeState) + i*4 + j)) * stationaryDist[j];

        siteLogLikelihoods[i] = std::log(siteLogL);
    }

    double alignmentLikelihood = 0.0;
    for(int i = 0; i < 4; i++)
        alignmentLikelihood += siteLogLikelihoods[i];
    
    delete [] siteLogLikelihoods;

    return alignmentLikelihood;
}