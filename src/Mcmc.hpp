#ifndef MCMC_HPP
#define MCMC_HPP

class AbstractDistribution;
class PhyloCTMC;
class MoveScheduler;

class Mcmc{
    public:
        Mcmc(void)=delete;
        Mcmc(int nC, int pF, int sF, int tI, AbstractDistribution* lD, AbstractDistribution* pD, MoveScheduler* m);
        void run(bool tune = false);
    private:
        int numCycles;
        int printFreq;
        int sampleFreq;
        int tuningInterval;
        AbstractDistribution* likelihood;
        AbstractDistribution* prior;
        void sampleChain(int n);
        MoveScheduler* moveScheduler;
};

#endif