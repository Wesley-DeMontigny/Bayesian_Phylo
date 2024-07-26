#ifndef MOVE_SCHEDULER_HPP
#define MOVE_SCHEDULER_HPP
#include <vector>

class TreeParameter;
class Move;

//For now this is hardset to only be moves on the tree
class MoveScheduler {
    public:
        MoveScheduler(void);
        MoveScheduler(std::vector<Move*> moves);
        Move* getMove();
        void registerMove(Move* m);
        //void registerMove(AbstractMove* m, int w);
        void tune();
    private:
        //std::vector<int> weights;
        std::vector<Move*> moveHandlers;
};

#endif