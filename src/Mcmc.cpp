#include "Mcmc.hpp"
#include "PhyloCTMC.hpp"
#include "RandomVariable.hpp"
#include <cmath>
#include <iostream>

Mcmc::Mcmc(int nC, int pF, int sF, PhyloCTMC* m) : numCycles(nC), printFreq(pF), sampleFreq(sF), model(m){   }


void Mcmc::run(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    double currentLnLikelihood = model->lnLikelihood();
    double currentLnPrior = model->lnPrior();

    for(int n = 1; n <= numCycles; n++){
        double lnProposalRatio = model->update();
        double newLnPrior = model->lnPrior();
        double newLnLikelihood = model->lnLikelihood();

        double lnPriorRatio = newLnPrior - currentLnPrior;
        double lnLikelihoodRatio = newLnLikelihood - currentLnLikelihood;
        double lnR = lnProposalRatio + lnPriorRatio + lnLikelihoodRatio;

        bool acceptMove = false;
        if(std::log(rng.uniformRv()) < lnR)
            acceptMove = true;

        if(n % printFreq == 0)
            std::cout << n << " " << currentLnLikelihood << " -> " << newLnLikelihood << std::endl;

        if(acceptMove == true){
            model->accept();
            currentLnLikelihood = newLnLikelihood;
            currentLnPrior = newLnPrior;
        }
        else{
            model->reject();
        }

        if(n % sampleFreq == 0)
            sampleChain(n);
    }
}

void Mcmc::sampleChain(int n){

}