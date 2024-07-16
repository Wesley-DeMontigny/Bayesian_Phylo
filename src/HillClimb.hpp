#ifndef HILL_CLIMB_HPP
#define HILL_CLIMB_HPP

class MoveScheduler;
class AbstractDistribution;

class HillClimb{
    public:
        HillClimb(void)=delete;
        HillClimb(int nC, int pF, int sF, AbstractDistribution* lD, MoveScheduler* m);
        void run();
    private:
        int numCycles;
        int printFreq;
        int sampleFreq;
        AbstractDistribution* likelihood;
        void sample(int n);
        MoveScheduler* moveScheduler;
};

#endif