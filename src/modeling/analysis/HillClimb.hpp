#ifndef HILL_CLIMB_HPP
#define HILL_CLIMB_HPP

class MoveScheduler;
class LikelihoodNode;

class HillClimb{
    public:
        HillClimb(void)=delete;
        HillClimb(int nC, int pF, LikelihoodNode* lD, MoveScheduler* mS);
        void run();
    private:
        MoveScheduler* moveScheduler;
        int numCycles;
        int printFreq;
        LikelihoodNode* likelihood;
};

#endif