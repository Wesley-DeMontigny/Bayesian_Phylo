#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <limits>
#include <ctime>
#include "Msg.hpp"
#include "RandomVariable.hpp"



RandomVariable::RandomVariable(void) {

    uint32_t seed = (uint32_t) time(NULL);
    initialize(seed);
}

RandomVariable::RandomVariable(uint32_t seed) {

    initialize(seed);
}

uint32_t RandomVariable::extractU32(void) {

    int i = index;
    if (index >= N)
        {
        twist();
        i = index;
        }

    uint32_t y = mt[i];
    index = i + 1;

    y ^= (mt[i] >> U);
    y ^= (y << S) & B;
    y ^= (y << T) & C;
    y ^= (y >> L);

    return y;
}


void RandomVariable::initialize(uint32_t seed) {

    mt[0] = seed;
    for (uint32_t i=1; i<N; i++)
        {
        mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
        }
    index = N;
    
    for (size_t i=0; i<10000; i++)
        extractU32();
}



void RandomVariable::twist(void) {

    for (uint32_t i=0; i<N; i++)
        {
        uint32_t x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);
        uint32_t xA = x >> 1;

        if ( x & 0x1 )
            xA ^= A;

        mt[i] = mt[(i + M) % N] ^ xA;
        }
    index = 0;
}

double RandomVariable::uniformRv(void) {

    return (double)extractU32() / UINT32_MAX;
}

double RandomVariable::exponentialRv(double lambda) {
    return -log(uniformRv())/lambda;
}

double RandomVariable::normalRv(double mu, double sigma) {
    double u1 = uniformRv();
    double u2 = uniformRv();
    //Generate single Box-Muller transform
    double z = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
    return mu + (std::sqrt(sigma) * z);
}

/* 
Generate using the Marsaglia and Tsang (2000) method:
(1) Setup: d a-1/3, c 1/sqrt(9d). 0.8 t c. 369 
(2) Generate v (1 cx)Ã3 with x normal. Repeat if v 0 [rare; requires x-sqrt(9a-3)]. 
(3) Generate uniform U. (4) If U 1-0.0331*xÃ4 return d*v. 
(5) If log(U) 0.5*xÃ2 d*(1-v log(v)) return d*v. 
(6) Go to step 2.
*/
double RandomVariable::gammaRv(double shape, double scale) {
    if(shape < 1.0){
        double u = uniformRv();
        return gammaRv(1.0 + shape, scale) * std::pow(u, 1.0 / shape);
    }

    double d = shape - 1.0 / 3.0;
    double c = 1.0 / std::sqrt(9.0 * d);

    while(true) {
        double x = normalRv(0.0, 1.0);
        double v = 1.0 + c * x;
        if(v > 0) {
            v = v* v * v;
            double x2 = x * x;
            double u = uniformRv();
            if (u < 1.0 - 0.0331 * x2 * x2) {
                return scale * d * v;
            }
            if (std::log(u) < 0.5 * x2 + d * (1.0 - v + std::log(v))) {
                return scale * d * v;
            }
        }
    }
}