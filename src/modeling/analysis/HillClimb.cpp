#include "HillClimb.hpp"
#include "modeling/likelihoods/LikelihoodNode.hpp"
#include "moves/MoveScheduler.hpp"
#include "moves/Move.hpp"
#include "events/EventManager.hpp"
#include <cmath>
#include <iostream>

HillClimb::HillClimb(LikelihoodNode* lD, MoveScheduler* mS) : likelihood(lD), moveScheduler(mS) {   }


void HillClimb::run(int numCycles, EventManager* e){

    likelihood->regenerate();
    likelihood->accept();
    likelihood->clean();
    double currentLnLikelihood = likelihood->lnLikelihood();

    for(int n = 1; n <= numCycles; n++){
        Move* m = moveScheduler->getMove();
        m->update();
        likelihood->regenerate();
        double newLnLikelihood = likelihood->lnLikelihood();

        if(newLnLikelihood > currentLnLikelihood){
            m->markAccepted();
            likelihood->accept();
            likelihood->clean();
            currentLnLikelihood = newLnLikelihood;
        }
        else{
            m->markRejected();
            likelihood->reject();
            likelihood->clean();
        }

    }
}


void HillClimb::run(EventManager* e){

    likelihood->regenerate();
    likelihood->accept();
    likelihood->clean();
    double currentLnLikelihood = likelihood->lnLikelihood();

    double checkpoint = currentLnLikelihood;
    int iteration = 0;
    while(true){
        Move* m = moveScheduler->getMove();
        m->update();
        likelihood->regenerate();
        double newLnLikelihood = likelihood->lnLikelihood();

        if(newLnLikelihood > currentLnLikelihood){
            m->markAccepted();
            likelihood->accept();
            likelihood->clean();
            currentLnLikelihood = newLnLikelihood;
        }
        else{
            m->markRejected();
            likelihood->reject();
            likelihood->clean();
        }

        iteration++;

        e->call(iteration);

        if(iteration % 1000 == 0){
            if(currentLnLikelihood - checkpoint < 0.5){
                break;
            }
            checkpoint = currentLnLikelihood;
        }
    }

}