#include <iostream>
#include "RandomVariable.hpp"
#include "Msg.hpp"
#include "Tree.hpp"
#include "Alignment.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();
    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");


    Tree t1(&rv, 10);
    Tree t2(&rv, 3.0, 1.0, 2.0);
    std::cout << "Original T1:\n" << t1.getNewick() << std::endl;

    t1 = t2;
    std::cout << "Changed T1:\n" << t1.getNewick() << std::endl;
}
