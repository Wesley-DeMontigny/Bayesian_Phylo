#include <cmath>
#include "JC69.hpp"

JC69::JC69(double l) : TreeModel(), lambda(l) {
    setStationaryDist({0.25, 0.25, 0.25, 0.25});
}

JC69::JC69() : TreeModel(), lambda(1) {
    setStationaryDist({0.25, 0.25, 0.25, 0.25});
}

std::vector<std::vector<double>> JC69::transitionProbability(double time){

    double p0 = std::log(0.25 + (0.75 * std::exp(-4 * lambda * time)));
    double p1 = std::log(0.25 - (0.25 * std::exp(-4 * lambda * time)));

    std::vector<std::vector<double>> pMatrix = 
                            {
                                {p0, p1, p1, p1},
                                {p1, p0, p1, p1},
                                {p1, p1, p0, p1},
                                {p1, p1, p1, p0}
                            };
    return pMatrix;
}
