#ifndef HILL_CLIMB_HPP
#define HILL_CLIMB_HPP

class MoveScheduler;
class AbstractLikelihood;

class HillClimb{
    public:
        HillClimb(void)=delete;
        HillClimb(int nC, int pF, AbstractLikelihood* lD, MoveScheduler* mS);
        void run();
    private:
        MoveScheduler* moveScheduler;
        int numCycles;
        int printFreq;
        AbstractLikelihood* likelihood;
};

#endif