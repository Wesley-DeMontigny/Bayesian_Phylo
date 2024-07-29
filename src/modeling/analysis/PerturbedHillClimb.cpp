#include "PerturbedHillClimb.hpp"
#include "modeling/likelihoods/LikelihoodNode.hpp"
#include "moves/MoveScheduler.hpp"
#include "moves/Move.hpp"
#include "events/EventManager.hpp"
#include "events/Event.hpp"
#include <cmath>
#include <iostream>

PerturbedHillClimb::PerturbedHillClimb(LikelihoodNode* lD, MoveScheduler* mS) : likelihood(lD), moveScheduler(mS) {   }

// This is similar to IQTree's Stochastic NNI, but it differs in that it does not return to one of the best candidates each run.
void PerturbedHillClimb::run(int qC, EventManager* e, Event* bME){

    double bestLnLikelihood = -INFINITY;
    int count = 0;
    while(count < qC){
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

        if(currentLnLikelihood > bestLnLikelihood){
            bestLnLikelihood = currentLnLikelihood;
            count = 0;
            bME->call(-1);
        }
        else{
            count++;
        }

        for(int i = 0; i < 100; i++){
            Move* m = moveScheduler->getMove();
            m->update();
        }

        moveScheduler->clearRecord();
    }
}