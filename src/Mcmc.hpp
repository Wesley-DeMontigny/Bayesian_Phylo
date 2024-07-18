#ifndef MCMC_HPP
#define MCMC_HPP

class AbstractLikelihood;
class PhyloCTMC;
class MoveScheduler;
class AbstractPrior;
class EventManager;

class Mcmc{
    public:
        Mcmc(void)=delete;
        Mcmc(AbstractLikelihood* lD, AbstractPrior* pD, MoveScheduler* mS);
        void run(int numCycles, EventManager* e);
    private:
        MoveScheduler* moveScheduler;
        AbstractLikelihood* likelihood;
        AbstractPrior* prior;
};

#endif