#include "Mcmc.hpp"
#include "RandomVariable.hpp"
#include "AbstractLikelihood.hpp"
#include "AbstractParameter.hpp"
#include "AbstractPrior.hpp"
#include "MoveScheduler.hpp"
#include "AbstractMove.hpp"
#include "EventManager.hpp"
#include <cmath>
#include <iostream>

Mcmc::Mcmc(AbstractLikelihood* lD, AbstractPrior* pD, MoveScheduler* mS) : likelihood(lD), prior(pD), moveScheduler(mS) {   }

void Mcmc::run(int numCycles, EventManager* e){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    likelihood->regenerateLikelihood();
    likelihood->acceptLikelihood();
    double currentLnLikelihood = likelihood->lnLikelihood();
    prior->regeneratePrior();
    prior->acceptPrior();
    double currentLnPrior = prior->lnPrior();

    for(int n = 1; n <= numCycles; n++){
        AbstractMove* m = moveScheduler->getMove();
        double lnProposalRatio = m->update();
        double newLnPrior = prior->lnPrior();
        double newLnLikelihood = likelihood->lnLikelihood();

        double lnPriorRatio = newLnPrior - currentLnPrior;
        double lnLikelihoodRatio = newLnLikelihood - currentLnLikelihood;
        double lnR = lnProposalRatio + lnPriorRatio + lnLikelihoodRatio;

        bool acceptMove = false;
        if(std::log(rng.uniformRv()) < lnR)
            acceptMove = true;

        if(acceptMove == true){
            m->accept();
            currentLnLikelihood = newLnLikelihood;
            currentLnPrior = newLnPrior;
        }
        else{
            m->reject();
        }

        e->call(n);
    }
}
