#ifndef HILL_CLIMB_HPP
#define HILL_CLIMB_HPP

class MoveScheduler;
class LikelihoodNode;
class EventManager;

class HillClimb{
    public:
        HillClimb(void)=delete;
        HillClimb(LikelihoodNode* lD, MoveScheduler* mS);
        void run(int numCycles, EventManager* e);
        void run(EventManager* e);
    private:
        MoveScheduler* moveScheduler;
        LikelihoodNode* likelihood;
};

#endif