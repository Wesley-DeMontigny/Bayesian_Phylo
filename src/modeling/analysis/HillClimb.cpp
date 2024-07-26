#include "HillClimb.hpp"
#include "AbstractLikelihood.hpp"
#include "AbstractParameter.hpp"
#include "MoveScheduler.hpp"
#include "AbstractMove.hpp"
#include <cmath>
#include <iostream>

HillClimb::HillClimb(int nC, int pF, LikelihoodNode* lD, MoveScheduler* mS) : numCycles(nC), printFreq(pF), likelihood(lD), moveScheduler(mS) {   }


void HillClimb::run(){

    likelihood->regenerateLikelihood();
    likelihood->acceptLikelihood();
    double currentLnLikelihood = likelihood->lnLikelihood();

    for(int n = 1; n <= numCycles; n++){
        Move* m = moveScheduler->getMove();
        m->update();
        double newLnLikelihood = likelihood->lnLikelihood();

        bool acceptMove = false;
        if(newLnLikelihood > currentLnLikelihood)
            acceptMove = true;

        if(n % printFreq == 0)
            std::cout << n << " " << currentLnLikelihood << " -> " << newLnLikelihood << std::endl;

        if(acceptMove == true){
            m->accept();
            currentLnLikelihood = newLnLikelihood;
        }
        else{
            m->reject();
        }

    }
}
