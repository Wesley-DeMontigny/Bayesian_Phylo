#include <iostream>
#include "RandomVariable.hpp"
#include "Tree.hpp"
#include "Alignment.hpp"
#include "EvolutionaryModel.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();
    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");
    //Tree t1(&rv, &aln);

    EvolutionaryModel model(&aln, &rv);

    std::cout << model.lnLikelihood() << std::endl;
}
