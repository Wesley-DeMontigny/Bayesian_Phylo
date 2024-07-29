#include "MoveScheduler.hpp"
#include "modeling/parameters/trees/TreeParameter.hpp"
#include "modeling/parameters/trees/TreeObject.hpp"
#include "core/RandomVariable.hpp"
#include "Move.hpp"
#include <iostream>

MoveScheduler::MoveScheduler(void) {}

MoveScheduler::MoveScheduler(std::vector<Move*> moves) : moveHandlers(moves) {}

// Just pick a random move for now 
Move* MoveScheduler::getMove(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();

    Move* pickedMove = moveHandlers[(int)(rng.uniformRv() * moveHandlers.size())];

    return pickedMove;
}

void MoveScheduler::registerMove(Move* m){
    moveHandlers.push_back(m);
}

void MoveScheduler::clearRecord(){
    for(Move* m : moveHandlers)
        m->clearRecord();
}

void MoveScheduler::tune(){
    for(Move* m : moveHandlers)
        m->tune();

}