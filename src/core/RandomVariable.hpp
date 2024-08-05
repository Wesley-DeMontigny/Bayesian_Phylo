#ifndef RANDOM_VARIABLE_HPP
#define RANDOM_VARIABLE_HPP

#include <cstdint>
#include <vector>

enum {
    N = 624,
    M = 397,
    R = 31,
    A = 0x9908B0DF,
    F = 1812433253,
    U = 11,
    S = 7,
    B = 0x9D2C5680,
    T = 15,
    C = 0xEFC60000,
    L = 18,
    MASK_LOWER = (1ull << R) - 1,
    MASK_UPPER = (1ull << R)
};

class RandomVariable {

    public:
        static RandomVariable&  randomVariableInstance(void) // Returns reference to RandomVariable object
                                    {
                                    static RandomVariable singleRandomVariable; // singleRandomVariable is initialized once
                                    return singleRandomVariable;
                                    }
        double                  uniformRv(void);
        double                  exponentialRv(double lamda);
        double                  gammaRv(double shape, double rate);
        double                  normalRv(double mu, double sigma);

    private:
                                RandomVariable(void);
                                RandomVariable(RandomVariable& r);
                                RandomVariable(uint32_t seed);
        RandomVariable&         operator=(const RandomVariable&);
        uint32_t                extractU32(void);
        void                    initialize(uint32_t seed);
        void                    twist(void);
        uint16_t                index;
        uint32_t                mt[N];
};

#endif
