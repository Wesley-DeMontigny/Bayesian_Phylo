#ifndef MCMC_HPP
#define MCMC_HPP

class PhyloCTMC;

class Mcmc{
    public:
        Mcmc(void)=delete;
        Mcmc(int nC, int pF, int sF, PhyloCTMC* m);
        void run();
    private:
        int numCycles;
        int printFreq;
        int sampleFreq;
        PhyloCTMC* model;
        void sampleChain(int n);
};

#endif