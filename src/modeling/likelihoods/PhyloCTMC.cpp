#include "PhyloCTMC.hpp"
#include "core/RandomVariable.hpp"
#include "core/Alignment.hpp"
#include "core/Msg.hpp"
#include "ConditionalLikelihood.hpp"
#include "TransitionProbability.hpp"
#include "modeling/parameters/trees/Node.hpp"
#include "modeling/parameters/trees/TreeObject.hpp"
#include "modeling/parameters/trees/TreeParameter.hpp"
#include "modeling/parameters/rates/RateMatrix.hpp"
#include <cmath>
#include <string>

PhyloCTMC::PhyloCTMC(Alignment* a, TreeParameter* t, RateMatrix* m) : aln(a), tree(t), rateMatrix(m), oldLikelihood(0.0), currentLikelihood(0.0) {

    this->dirty();

    TreeObject* activeT = tree->getTree();
    stateSpace = aln->getStateSpace();
    if(aln->getNumTaxa() != activeT->getNumTaxa())
        Msg::error("Expected " + std::to_string(aln->getNumTaxa()) + 
        "taxa in the tree, but found only " + std::to_string(activeT->getNumTaxa()));
    
    int numNodes = aln->getNumTaxa() * 2 - 1;
    activeCL = new bool[2 * numNodes];
    activeTP = new bool[2 * numNodes];
    for(int i = 0; i < 2 * numNodes; i++){
        activeCL[i] = false;
        activeTP[i] = false;
    }

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
            Msg::warning("Expected to find a sequence named " + name + "! Assigning indices and names randomly.");
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

    tree->accept(); //Accept the tip changes into memory tree (if any happened)

    condL = new ConditionalLikelihood(aln);
    transProb = new TransitionProbability(2 * aln->getNumTaxa() - 1, stateSpace);

    activeT->updateAll();
}


PhyloCTMC::~PhyloCTMC(){
    delete condL;
    delete transProb;
    delete [] activeCL;
    delete [] activeTP;
}

void PhyloCTMC::accept() {
    oldLikelihood = currentLikelihood;

    int numNodes = aln->getNumTaxa() * 2 - 1;
    for(int i = 0; i < numNodes; i++){
        activeCL[i + numNodes] = activeCL[i];
        activeTP[i + numNodes] = activeTP[i];
    }

    tree->accept();
    tree->clean();
    rateMatrix->accept();
    rateMatrix->clean();
}

void PhyloCTMC::reject() {
    currentLikelihood = oldLikelihood;

    int numNodes = aln->getNumTaxa() * 2 - 1;
    for(int i = 0; i < numNodes; i++){
        activeCL[i] = activeCL[i + numNodes];
        activeTP[i] = activeTP[i + numNodes];
    }

    tree->reject();
    tree->clean();
    rateMatrix->reject();
    rateMatrix->clean();
}

void PhyloCTMC::regenerate(){
    tree->regenerate();
    rateMatrix->regenerate();
    if(tree->isDirty() || rateMatrix->isDirty())
        this->dirty();

    if(this->isDirty()){
        bool updateAllTPs = rateMatrix->isDirty();
        TreeObject* activeT = tree->getTree();

        std::vector<Node*>&  poSeq = activeT->getPostOrderSeq();

        for(Node* n : poSeq){
            //Only update the conditional likelihoods if the node has changed
            int nIndex = n->getIndex();
            if(updateAllTPs == true || n->getNeedsTPUpdate() == true){
                if(n != activeT->getRoot()){
                    activeTP[nIndex] ^= true;
                    double v = activeT->getBranchLength(n);
                    transProb->set(activeTP[nIndex], nIndex, rateMatrix->P(v));
                }
                n->setNeedsTPUpdate(false);
            }
            if(n->getNeedsCLUpdate() == true || n->getNeedsTPUpdate() == true){
                //Get memory address of the node we are looking at and pre-set all of the likelihoods at each site to be 1.0
                activeCL[nIndex] ^= true;
                double* pNN = (*condL)(nIndex, activeCL[nIndex]);
                std::fill(pNN, pNN + (aln->getNumChar() * stateSpace), 1.0);

                std::set<Node*>& nNeighbors = n->getNeighbors();
                //Iterate over the descendents (usually only two)
                for(Node* d : nNeighbors){
                    if(d != n->getAncestor()){
                        int dIndex = d->getIndex();
                        double* pN = pNN;
                        double* pD = (*condL)(dIndex, activeCL[dIndex]);
                        DoubleMatrix P = *(*transProb)(activeTP[dIndex], dIndex);

                        //Iterate over each of the characters and each of the potential states of our node
                        for(int c = 0, len=aln->getNumChar(); c < len; c++){
                            for(int i = 0; i < stateSpace; i++){
                                //Sum up the products of the likelihoods from the CTMC (transitioning from the node's hypothetical state to another) and the conditional likelihood of the descendent states 
                                double sum = 0.0;
                                for(int j = 0; j < stateSpace; j++){
                                    sum += P(i, j) * pD[j];
                                }
                                //If this is the first time, set pN equal to the sum, otherwise multiply them
                                (*pN) *= sum;
                                //Move the memory address to the next character state
                                pN++;
                            }
                            //Move the memory address to the next site
                            pD+=stateSpace;
                        }
                    }
                }
                //Note that we have updated the node
                n->setNeedsCLUpdate(false);
            }
        }

        //Calculate the likelihood of the tree by summing up the likelihood at the root.
        int rIndex = activeT->getRoot()->getIndex();
        double* pR = (*condL)(rIndex, activeCL[rIndex]);
        std::vector<double>& f = transProb->getStationaryFreq();
        double lnL = 0.0;

        for(int c = 0, len=aln->getNumChar(); c < len; c++){
            double like = 0.0;
            for(int i = 0; i < stateSpace; i++){
                like += pR[i]*f[i];
            }
            lnL += std::log(like);
            pR += stateSpace;//Go to the next site...
        }

        currentLikelihood = lnL;
    }
}