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
            /*
            The handling of the tips is somewhat easy, but we need to remember that we will mostly be dealing
            with stateLikelihoods of 0 or 1, so when we represent it as a log, we need to handle it accordingly.
            */
            if(n->getIsTip()){
                std::vector<double> stateLikelihoods = {0,0,0,0};
                int state = dataMatrix[alignmentIndex][i];
                
                double stateCount = 0;
                for(int j = 0; j < 4; j++) {
                    stateLikelihoods[j] = (state >> j & 1);
                    stateCount += stateLikelihoods[j];

                }

                for(int k = 0; k < 4; k++)
                        stateLikelihoods[k] = stateLikelihoods[k] == 0 ? -1 * INFINITY : std::log(stateLikelihoods[k] / stateCount);

                L[n->getIndex()] = stateLikelihoods;
            }
            /*
            If it isn't a tip, then we need to use the internal node likelihood function:
            This is the product of giving rise to your daughters given a certain state,
            which itself invokes the likelihood of the daughters. See p. 104 of Molecular
            Evolution: A Statistical Approach (Yang 2014)
            */
            else{
                std::vector<double> stateLikelihoods = {0,0,0,0};
                std::set<Node*> neighbors = n->getNeighbors();

                for(int j = 0; j < 4; j++){
                    double stateLogL = 0;
                    for(Node* neighbor : neighbors){
                        if(neighbor != n->getAncestor()){
                            //Get the transition probability from the map. In this case n is the ancestor
                            auto itTP = transitionProbMap.find({neighbor, n});
                            std::vector<std::vector<double>> P = itTP->second;

                            //I need to Log-Sum-Exp here
                            double daughterPathL = 0;
                            for(int k = 0; k < 4; k++){
                                daughterPathL += std::exp(P[j][k] + L[neighbor->getIndex()][k]);
                            }
                            stateLogL += std::log(daughterPathL);
                        }
                    }
                    stateLikelihoods[j] = stateLogL;
                }

                L[n->getIndex()] = stateLikelihoods;
            }
        }
        /*
        Final likelihood is summing over the product of the likelihoods
        up to the root and the stationary distribution. I also need to do
        Log-Sum-Exp here.
        */
        double siteLogL = 0.0;
        std::vector<double> rootLogL = L[t.getRoot()->getIndex()];
        for(int j = 0; j < 4; j++)
        {
            siteLogL += std::exp(rootLogL[j] + std::log(stationaryDist[j]));
        }
        siteLogLikelihoods[i] = std::log(siteLogL);

    }

    double alignmentLikelihood = 0.0;
    for(double sLL : siteLogLikelihoods)
        alignmentLikelihood += sLL;
    
    return alignmentLikelihood;
}