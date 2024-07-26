#include "Mcmc.hpp"
#include "core/RandomVariable.hpp"
#include "modeling/PosteriorNode.hpp"
#include "moves/MoveScheduler.hpp"
#include "moves/Move.hpp"
#include "events/EventManager.hpp"
#include <cmath>
#include <iostream>

Mcmc::Mcmc(PosteriorNode* pN, MoveScheduler* mS) : posterior(pN), moveScheduler(mS) {   }

void Mcmc::run(int numCycles, EventManager* e){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    posterior->regenerate();
    posterior->accept();
    double currentLnPosterior = posterior->lnPosterior();

    for(int n = 1; n <= numCycles; n++){
        Move* m = moveScheduler->getMove();
        double lnProposalRatio = m->update();
        posterior->regenerate();
        double newLnPosterior = posterior->lnPosterior();

        double lnPosteriorRatio = newLnPosterior - currentLnPosterior;
        double lnR = lnProposalRatio + lnPosteriorRatio;

        bool acceptMove = false;
        if(std::log(rng.uniformRv()) < lnR)
            acceptMove = true;

        if(acceptMove == true){
            m->markAccepted();
            posterior->accept();
        }
        else{
            m->markRejected();
            posterior->reject();
        }

        e->call(n);
    }
}
