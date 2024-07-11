#include "PhyloCTMC.hpp"
#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "Msg.hpp"
#include "ConditionalLikelihood.hpp"
#include "TransitionProbability.hpp"
#include "Node.hpp"
#include "TreeObject.hpp"
#include "TreeParameter.hpp"
#include <cmath>
#include <string>

PhyloCTMC::PhyloCTMC(Alignment* a, TreeParameter* t) : aln(a), tree(t) {

    TreeObject* activeT = tree->getValue();

    if(aln->getNumTaxa() != activeT->getNumTaxa())
        Msg::error("Error: Expected " + std::to_string(aln->getNumTaxa()) + 
        "taxa in the tree, but found only " + std::to_string(activeT->getNumTaxa()));

    //We need to do some setting to make sure the alignment and tree match
    std::vector<std::string> taxaNames = aln->getTaxaNames();
    bool randomAssign = false;
    for(Node* n : activeT->getTips()){
        bool found = false;
        std::string name = n->getName();
        for(int i = 0; i < taxaNames.size(); i++){
            if(name == taxaNames[i]){
                found=true;
                n->setIndex(i);
                break;
            }
        }

        if(found == false){
            Msg::warning("Warning: Expected to find a tip named " + name + "! Assigning indices and names randomly.");
            randomAssign = true;
            break;
        }
    }

    //Set the tips randomly if the tips are not properly named
    if(randomAssign == true){
        std::vector<Node*> tips = activeT->getTips();
        for(int i = 0; i < taxaNames.size(); i++){
            tips[i]->setIndex(i);
            tips[i]->setName(taxaNames[i]);
        }
    }

    tree->accept(); //Accept the tip changes into memory (if any happened)

    condL = new ConditionalLikelihood(aln);
    transProb = new TransitionProbability(2 * aln->getNumTaxa() - 1);

    activeT->updateAll();
    double lnL = lnLikelihood();
}


PhyloCTMC::~PhyloCTMC(){
    delete condL;
    delete transProb;
}

double PhyloCTMC::lnLikelihood(){

    TreeObject* activeT = tree->getValue();

    std::vector<Node*>&  dpSeq = activeT->getPostOrderSeq();
    for(Node* n : dpSeq){
        //Only update the conditional likelihoods if the node has changed
        if(n->getNeedsTPUpdate() == true){
            n->flipTP();
            if(n != activeT->getRoot()){
                double v = activeT->getBranchLength(n, n->getAncestor());
                transProb->set(n->getActiveTP(), n->getIndex(), v);
                n->setNeedsTPUpdate(false);
            }
        }
        if(n->getNeedsCLUpdate() == true){
            n->flipCL();
            //Get memory address of the node we are looking at and pre-set all of the likelihoods at each site to be 1.0
            double* pNN = (*condL)(n->getIndex(), n->getActiveCL());
            for(int c = 0, len=aln->getNumChar()*4; c < len; c++) 
                pNN[c] = 1.0;

            std::set<Node*>& nNeighbors = n->getNeighbors();
            //Iterate over the descendents (usually only two)
            for(Node* d : nNeighbors){
                if(d != n->getAncestor()){
                    double* pN = pNN;
                    double* pD = (*condL)(d->getIndex(), d->getActiveCL());
                    DoubleMatrix* P = (*transProb)(d->getIndex(), d->getActiveTP());

                    //Iterate over each of the characters and each of the potential states of our node
                    for(int c = 0, len=aln->getNumChar(); c < len; c++){
                        for(int i = 0; i < 4; i++){
                            //Sum up the products of the likelihoods from the CTMC (transitioning from the node's hypothetical state to another) and the conditional likelihood of the descendent states 
                            double sum = 0.0;
                            for(int j = 0; j < 4; j++){
                                sum += (*P)(i, j) * pD[j];
                            }
                            //If this is the first time, set pN equal to the sum, otherwise multiply them
                            (*pN) *= sum;
                            //Move the memory address to the next character state
                            pN++;
                        }
                        //Move the memory address to the next site
                        pD+=4;
                    }
                }
            }
            //Note that we have updated the node
            n->setNeedsCLUpdate(false);
        }
    }
    
    //Calculate the likelihood of the tree by summing up the likelihood at the root.
    double* pR = (*condL)(activeT->getRoot()->getIndex(), activeT->getRoot()->getActiveCL());
    std::vector<double>& f = transProb->getStationaryFreq();
    double lnL = 0.0;

    for(int c = 0, len=aln->getNumChar(); c < len; c++){
        double like = 0.0;
        for(int i = 0; i < 4; i++){
            like += pR[i]*f[i];
        }
        lnL += std::log(like);
    }

    return lnL;
}