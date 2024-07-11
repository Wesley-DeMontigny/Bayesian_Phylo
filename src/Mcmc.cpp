#include "Mcmc.hpp"
#include "PhyloCTMC.hpp"
#include "RandomVariable.hpp"
#include "AbstractDistribution.hpp"
#include "AbstractParameter.hpp"
#include "MoveScheduler.hpp"
#include "AbstractMove.hpp"
#include <cmath>
#include <iostream>

Mcmc::Mcmc(int nC, int pF, int sF, AbstractDistribution* lD, AbstractDistribution* pD, MoveScheduler* m) : numCycles(nC), printFreq(pF), sampleFreq(sF), likelihood(lD), prior(pD), moveScheduler(m) {   }


void Mcmc::run(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    double currentLnLikelihood = likelihood->lnLikelihood();
    double currentLnPrior = prior->lnLikelihood();

    for(int n = 1; n <= numCycles; n++){
        AbstractMove* m = moveScheduler->getMove();
        double lnProposalRatio = m->update();
        double newLnPrior = prior->lnLikelihood();
        double newLnLikelihood = likelihood->lnLikelihood();

        double lnPriorRatio = newLnPrior - currentLnPrior;
        double lnLikelihoodRatio = newLnLikelihood - currentLnLikelihood;
        double lnR = lnProposalRatio + lnPriorRatio + lnLikelihoodRatio;

        bool acceptMove = false;
        if(std::log(rng.uniformRv()) < lnR)
            acceptMove = true;

        if(n % printFreq == 0)
            std::cout << n << " " << currentLnLikelihood << " -> " << newLnLikelihood << std::endl;

        if(acceptMove == true){
            m->getParameter()->accept();
            currentLnLikelihood = newLnLikelihood;
            currentLnPrior = newLnPrior;
        }
        else{
            m->getParameter()->reject();
        }

        if(n % sampleFreq == 0)
            sampleChain(n);
    }
}

void Mcmc::sampleChain(int n){

}