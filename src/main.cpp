#include <iostream>
#include <vector>
#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "TreeParameter.hpp"
#include "TreeObject.hpp"
#include "MoveScheduler.hpp"
#include "FlatPrior.hpp"
#include "AbstractMove.hpp"
#include "MoveTreeNNI.hpp"
#include "MoveScaleBranch.hpp"
#include "PhyloCTMC.hpp"
#include "Mcmc.hpp"
#include "ExponentialDistribution.hpp"
#include "CompoundDistribution.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rng = RandomVariable::randomVariableInstance();

    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    TreeObject treeObj(&aln);
    TreeParameter treeParam = TreeParameter(23);
    PhyloCTMC model(&aln, &treeParam);
    auto branchLengths = std::bind(&TreeParameter::getBranchLengths, &treeParam);
    ExponentialDistribution branchPrior(0.2, branchLengths);

    std::vector<AbstractMove*> moves {
        &MoveTreeNNI(&treeParam), 
        &MoveScaleBranch(&treeParam)
    };
    MoveScheduler moveScheduler(moves);

    Mcmc myMCMC(10000, 1, 1, &model, &branchPrior, &moveScheduler);
    myMCMC.run();

    std::cout << treeParam.getTree()->getNewick() << std::endl;
}
