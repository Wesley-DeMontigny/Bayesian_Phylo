#include <iostream>
#include "RandomVariable.hpp"
#include "Msg.hpp"
#include "Tree.hpp"
#include "Alignment.hpp"
#include "TreeLikelihood.hpp"
#include "JC69.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();
    Alignment* aln = new Alignment("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");
    Tree* t1 = new Tree(&rv, aln);

    JC69* model = new JC69(t1, aln, {0.25, 0.25, 0.25, 0.25});
    model->initializeTransitionProbabilityMap();

    std::cout << "Pruning..." << std::endl;
    double L1 = TreeLikelihood::FelPrune(model);
    std::cout << "Alignment Log Likelihood Given Random Tree: " << L1 << std::endl;
}
