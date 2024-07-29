#ifndef PERTURBED_HILL_CLIMB_HPP
#define PERTURBED_HILL_CLIMB_HPP

class MoveScheduler;
class LikelihoodNode;
class EventManager;
class Event;

class PerturbedHillClimb{
    public:
        PerturbedHillClimb(void)=delete;
        PerturbedHillClimb(LikelihoodNode* lD, MoveScheduler* mS);
        void run(int qC, EventManager* e, Event* bME);
    private:
        MoveScheduler* moveScheduler;
        LikelihoodNode* likelihood;
};

#endif