#include <iostream>
#include "RandomVariable.hpp"
#include "Msg.hpp"
#include "Tree.hpp"
#include "Alignment.hpp"
#include "TreeLikelihood.hpp"
#include "JC69.hpp"
#include "DoubleMatrix.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();
    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");
    Tree t1(&rv, &aln);
    
    double* stationaryDist = new double[4]{0.25, 0.25, 0.25, 0.25};
    JC69 model(&t1, &aln, stationaryDist);
    model.initializeTransitionProbabilityMap();

    std::cout << "Pruning..." << std::endl;
    double L1 = TreeLikelihood::FelPrune(&model);
    std::cout << "Alignment Log Likelihood Given Random Tree: " << L1 << std::endl;

    delete stationaryDist;
}
