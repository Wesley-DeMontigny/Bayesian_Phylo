#include "TreeModel.hpp"
#include "Msg.hpp"
#include "ConditionalLikelihood.hpp"
#include <cmath>

TreeModel::TreeModel(void) {}

double TreeModel::FelPrune(Tree t, Alignment aln, bool debug){
    
    if(aln.getDataType() != NxsCharactersBlock::dna && aln.getDataType() != NxsCharactersBlock::rna && aln.getDataType() != NxsCharactersBlock::nucleotide)
        Msg::error("Non-nucleotide data is currently not implemented");

    std::vector<Node*> nodes = t.getPostOrderSeq();

    std::vector<double> siteLogLikelihoods;
    siteLogLikelihoods.reserve(aln.getNumChar());
    siteLogLikelihoods.resize(aln.getNumChar());
    std::vector<std::vector<int>> dataMatrix = aln.getMatrix();


    //Lets initialize the transition probability matrix ahead of time. 
    //This will save us from looking it up multiple times
    if(debug)
        std::cout << "Registering transition probabilities..." << std::endl;
    std::map<std::pair<Node*, Node*>, std::vector<std::vector<double>>> transitionProbMap;
    for(Node* n : nodes){
        if (n->getAncestor() != nullptr){
            transitionProbMap.insert(
                {{n, n->getAncestor()},
                transitionProbability(t.getBranchLength(n, n->getAncestor()))}
                );
        }
    }

    //Initialize conditional likelihood
    if(debug)
        std::cout << "Initializing conditional likelihood object..." << std::endl;
    ConditionalLikelihood condL = ConditionalLikelihood(t, aln);

    //Iterate site by site to get site log likelihoods
    if(debug)
        std::cout << "Starting Felsenstein pruning..." << std::endl;
    for(int i = 0; i < aln.getNumChar(); i++){

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
                        double childL = 0;
                        if(neighbor != n->getAncestor()){
                            //Get the transition probability from the map. In this case n is the ancestor
                            auto itTP = transitionProbMap.find({neighbor, n});
                            std::vector<std::vector<double>> P = itTP->second;

                            int neighborIndex = neighbor->getIndex();

                            //Iterate over children states
                            for(int k = 0; k < 4; k++)
                                childL += (P[j][k] * *condL.getCondLikelihood(neighborIndex, i, k));

                            stateL *= childL;
                        }
                    }
                    *condL.getCondLikelihood(nIndex, i, j) = stateL;
                }
            }
        }

        //Final likelihood is summing over the product of the likelihoods up to the root and the stationary distribution.
        double siteLogL = 0.0;
        for(int j = 0; j < 4; j++)
            siteLogL += *condL.getRootConditionalLikelihood(i, j) * stationaryDist[j];

        siteLogLikelihoods[i] = std::log(siteLogL);
    }

    double alignmentLikelihood = 0.0;
    for(double sLL : siteLogLikelihoods)
        alignmentLikelihood += sLL;
    
    return alignmentLikelihood;
}