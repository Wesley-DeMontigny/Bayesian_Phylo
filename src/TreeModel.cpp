#include "TreeModel.hpp"
#include "Msg.hpp"
#include <cmath>

TreeModel::TreeModel(void) {}

double TreeModel::FelPrune(Tree t, Alignment aln){
    
    if(aln.getDataType() != NxsCharactersBlock::dna && aln.getDataType() != NxsCharactersBlock::rna && aln.getDataType() != NxsCharactersBlock::nucleotide)
        Msg::error("Non-nucleotide data is currently not implemented");

    std::vector<Node*> nodes = t.getPostOrderSeq();

    std::vector<double> siteLogLikelihoods;
    siteLogLikelihoods.reserve(aln.getNumChar());
    siteLogLikelihoods.resize(aln.getNumChar());
    std::vector<std::vector<int>> dataMatrix = aln.getMatrix();


    //Lets initialize the transition probability matrix ahead of time. 
    //This will save us from looking it up multiple times
    std::map<std::pair<Node*, Node*>, std::vector<std::vector<double>>> transitionProbMap;
    for(Node* n : nodes){
        if (n->getAncestor() != nullptr){
            transitionProbMap.insert(
                {{n, n->getAncestor()},
                transitionProbability(t.getBranchLength(n, n->getAncestor()))}
                );
        }
    }


    //Iterate site by site to get site log likelihoods
    for(int i = 0; i < aln.getNumChar(); i++){
        std::vector<std::vector<double>> L;
        L.reserve(nodes.size());
        L.resize(nodes.size());

        for(Node* n : nodes){
            int alignmentIndex = n->getAlignmentIndex();
            std::vector<double> stateLikelihoods = {0,0,0,0};
            //If it is a tip, then we just see what the alignment says
            if(n->getIsTip()){
                int state = dataMatrix[alignmentIndex][i];
                
                double stateCount = 0;
                for(int j = 0; j < 4; j++) {
                    stateLikelihoods[j] = (state >> j & 1);
                    stateCount += stateLikelihoods[j];
                }

                for(int k = 0; k < 4; k++)
                        stateLikelihoods[k] = stateLikelihoods[k] / stateCount;

                L[n->getIndex()] = stateLikelihoods;
            }
            /*
            If it isn't a tip, then we need to use the internal node likelihood function:
            This is the product of giving rise to your children given a certain state,
            which itself invokes the likelihood of the children. See p. 104 of Molecular
            Evolution: A Statistical Approach (Yang 2014)
            */
            else{
                std::set<Node*> neighbors = n->getNeighbors();

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

                            //Iterate over children states
                            for(int k = 0; k < 4; k++)
                                childL += (P[j][k] * L[neighbor->getIndex()][k]);

                            stateL *= childL;
                        }
                    }
                    stateLikelihoods[j] = stateL;
                }

                L[n->getIndex()] = stateLikelihoods;
            }
        }
        /*
        Final likelihood is summing over the product of the likelihoods
        up to the root and the stationary distribution.
        */
        double siteLogL = 0.0;
        std::vector<double> rootL = L[t.getRoot()->getIndex()];

        for(int j = 0; j < 4; j++)
            siteLogL += rootL[j] * stationaryDist[j];

        siteLogLikelihoods[i] = std::log(siteLogL);

    }

    double alignmentLikelihood = 0.0;
    for(double sLL : siteLogLikelihoods)
        alignmentLikelihood += sLL;
    
    return alignmentLikelihood;
}