#ifndef MOVE_SCHEDULER_HPP
#define MOVE_SCHEDULER_HPP
#include <vector>

class TreeParameter;
class AbstractMove;

//For now this is hardset to only be moves on the tree
class MoveScheduler {
    public:
        MoveScheduler(void);
        MoveScheduler(std::vector<AbstractMove*> moves);
        AbstractMove* getMove();
        void registerMove(AbstractMove* m){moveHandlers.push_back(m);}
        void tune();
    private:
        std::vector<AbstractMove*> moveHandlers;
};

#endif