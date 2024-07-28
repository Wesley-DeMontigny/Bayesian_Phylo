#include "HillClimb.hpp"
#include "modeling/likelihoods/LikelihoodNode.hpp"
#include "moves/MoveScheduler.hpp"
#include "moves/Move.hpp"
#include <cmath>
#include <iostream>

HillClimb::HillClimb(int nC, int pF, LikelihoodNode* lD, MoveScheduler* mS) : numCycles(nC), printFreq(pF), likelihood(lD), moveScheduler(mS) {   }


void HillClimb::run(){

    likelihood->regenerate();
    likelihood->accept();
    likelihood->clean();
    double currentLnLikelihood = likelihood->lnLikelihood();

    for(int n = 1; n <= numCycles; n++){
        Move* m = moveScheduler->getMove();
        m->update();
        likelihood->regenerate();
        double newLnLikelihood = likelihood->lnLikelihood();

        bool acceptMove = false;
        if(newLnLikelihood > currentLnLikelihood)
            acceptMove = true;

        if(n % printFreq == 0)
            std::cout << n << " " << currentLnLikelihood << " -> " << newLnLikelihood << std::endl;

        if(acceptMove == true){
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
