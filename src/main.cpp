#include <iostream>
#include "RandomVariable.hpp"
#include "Msg.hpp"
#include "Tree.hpp"
#include "Alignment.hpp"
#include "TreeModel.hpp"
#include "JC69.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();
    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    JC69 model;

    Tree t1(&rv, aln);
    double L1 = model.FelPrune(t1, aln);

    std::cout << "Alignment Log Likelihood Given Random Tree: " << L1 << std::endl;

}
