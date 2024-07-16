#include "HillClimb.hpp"
#include "AbstractDistribution.hpp"
#include "AbstractParameter.hpp"
#include "MoveScheduler.hpp"
#include "AbstractMove.hpp"
#include <cmath>
#include <iostream>

HillClimb::HillClimb(int nC, int pF, int sF, AbstractDistribution* lD,  MoveScheduler* m) : numCycles(nC), printFreq(pF), sampleFreq(sF), likelihood(lD), moveScheduler(m) {   }


void HillClimb::run(){

    double currentLnLikelihood = likelihood->lnLikelihood();

    for(int n = 1; n <= numCycles; n++){
        AbstractMove* m = moveScheduler->getMove();
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

        if(n % sampleFreq == 0)
            sample(n);
    }
}

void HillClimb::sample(int n){

}