#include <cmath>
#include "JC69.hpp"

JC69::JC69(void) : TreeModel() {
    setStationaryDist({0.25, 0.25, 0.25, 0.25});
}

/*
The Jukes-Cantor (1969) model is fairly easy to implement because it has
a nice analytical solution for its transition probability matrix. Although
the JC69 model is sometimes shown as having a parameter lambda for the rate,
in reality this parameter disappears from the analytical solution when we 
normalize the branch lengths so that it is in units of the average number of 
changes per site.
*/
std::vector<std::vector<double>> JC69::transitionProbability(double time){

    double p0 = 0.25 + (0.75 * std::exp((-4.0/3.0) * time));
    double p1 = 0.25 - (0.25 * std::exp((-4.0/3.0) * time));

    std::vector<std::vector<double>> pMatrix = 
                            {
                                {p0, p1, p1, p1},
                                {p1, p0, p1, p1},
                                {p1, p1, p0, p1},
                                {p1, p1, p1, p0}
                            };
    return pMatrix;
}
