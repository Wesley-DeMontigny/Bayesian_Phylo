#include "PhyloCTMC.hpp"
#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "ConditionalLikelihood.hpp"
#include "TransitionProbability.hpp"
#include "Node.hpp"
#include "Tree.hpp"
#include <cmath>

PhyloCTMC::PhyloCTMC(Alignment* a) : aln(a) {

    rng = &RandomVariable::randomVariableInstance();

    condL = new ConditionalLikelihood(aln);
    transProb = new TransitionProbability(2 * aln->getNumTaxa() - 1);
    tree[0] = new Tree(rng, aln);
    tree[1] = new Tree(*tree[0]);

    tree[0]->updateAll();
    double lnL = lnLikelihood();
}


PhyloCTMC::~PhyloCTMC(){
    delete condL;
    delete transProb;
}

double PhyloCTMC::lnLikelihood(){

    std::vector<Node*>&  dpSeq = tree[0]->getPostOrderSeq();
    for(Node* n : dpSeq){
        //Only update the conditional likelihoods if the node has changed
        if(n->getNeedsTPUpdate() == true){
            n->flipTP();
            if(n != tree[0]->getRoot()){
                double v = tree[0]->getBranchLength(n, n->getAncestor());
                transProb->set(n->getActiveTP(), n->getIndex(), v);
                n->setNeedsTPUpdate(false);
            }
        }
        if(n->getNeedsCLUpdate() == true){
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
    double* pR = (*condL)(tree[0]->getRoot()->getIndex(), tree[0]->getRoot()->getActiveCL());
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

double PhyloCTMC::lnPrior(){
    return 0.0;
}

double PhyloCTMC::update(){
    return tree[0]->update();
}

void PhyloCTMC::accept(){
    *tree[1] = *tree[0];
}

void PhyloCTMC::reject(){
    *tree[0] = *tree[1];
}