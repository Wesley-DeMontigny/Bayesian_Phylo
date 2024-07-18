#include "MoveScheduler.hpp"
#include "TreeParameter.hpp"
#include "TreeObject.hpp"
#include "RandomVariable.hpp"
#include "AbstractMove.hpp"
#include <iostream>

MoveScheduler::MoveScheduler(void) {}

MoveScheduler::MoveScheduler(std::vector<AbstractMove*> moves) : moveHandlers(moves) {}

// Just pick a random move for now 
AbstractMove* MoveScheduler::getMove(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    AbstractMove* pickedMove = moveHandlers[(int)(rng.uniformRv() * moveHandlers.size())];

    return pickedMove;
}

void MoveScheduler::registerMove(AbstractMove* m){
    moveHandlers.push_back(m);
    //weights.push_back(1.0);
}

void MoveScheduler::tune(){
    for(AbstractMove* m : moveHandlers)
        m->tune();

}