#include "CompoundDistribution.hpp"

CompoundDistribution::CompoundDistribution(void){}

CompoundDistribution::CompoundDistribution(std::vector<AbstractDistribution*> d) : distributions(d){}

void CompoundDistribution::addDistribution(AbstractDistribution* d){
    distributions.push_back(d);
}

double CompoundDistribution::lnLikelihood(){
    double lnL = 0.0;
    for(AbstractDistribution* a : distributions)
        lnL += a->lnLikelihood();
    return lnL;
}