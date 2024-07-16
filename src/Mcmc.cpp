#include "Mcmc.hpp"
#include "RandomVariable.hpp"
#include "AbstractDistribution.hpp"
#include "AbstractParameter.hpp"
#include "MoveScheduler.hpp"
#include "AbstractMove.hpp"
#include <cmath>
#include <iostream>

Mcmc::Mcmc(int nC, int pF, int sF, int tI, AbstractDistribution* lD, AbstractDistribution* pD) : numCycles(nC), printFreq(pF), sampleFreq(sF), tuningInterval(tI), likelihood(lD), prior(pD) {   }


void Mcmc::run(bool tune){
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    MoveScheduler& moveScheduler = MoveScheduler::moveSchedulerInstance();

    double currentLnLikelihood = likelihood->lnLikelihood();
    double currentLnPrior = prior->lnLikelihood();

    for(int n = 1; n <= numCycles; n++){
        AbstractMove* m = moveScheduler.getMove();
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
            m->accept();
            currentLnLikelihood = newLnLikelihood;
            currentLnPrior = newLnPrior;
        }
        else{
            m->reject();
        }

        if(tune == true && n % tuningInterval == 0)
            moveScheduler.tune();

        if(n % sampleFreq == 0)
            sampleChain(n);
    }
}

void Mcmc::sampleChain(int n){

}