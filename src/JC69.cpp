#include <cmath>
#include <vector>
#include "JC69.hpp"

JC69::JC69(Tree* t, Alignment* a, double* sD) : EvolutionaryModel(t, a, sD) {}

/*
The Jukes-Cantor (1969) model is fairly easy to implement because it has
a nice analytical solution for its transition probability matrix. Although
the JC69 model is sometimes shown as having a parameter lambda for the rate,
in reality this parameter disappears from the analytical solution when we 
normalize the branch lengths so that it is in units of the average number of 
changes per site.
*/
DoubleMatrix JC69::P(double time){

    double p0 = 0.25 + (0.75 * std::exp((-4.0/3.0) * time));
    double p1 = 0.25 - (0.25 * std::exp((-4.0/3.0) * time));


    DoubleMatrix pMatrix(4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(i == j)
                pMatrix(i,j) = p0;
            else
                pMatrix(i,j) = p1;
        }
    }
    
    return pMatrix;
}
