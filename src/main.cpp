#include <iostream>
#include <vector>
#include <string>
#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "TreeParameter.hpp"
#include "TreeObject.hpp"
#include "MoveScheduler.hpp"
#include "MoveTreeNNI.hpp"
#include "MoveScaleBranch.hpp"
#include "PhyloCTMC.hpp"
#include "Mcmc.hpp"
#include "HillClimb.hpp"
#include "ExponentialDistribution.hpp"
#include "NormalDistribution.hpp"
#include "CompoundDistribution.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rng = RandomVariable::randomVariableInstance();

    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    TreeParameter treeParam = TreeParameter(23);
    PhyloCTMC model(&aln, &treeParam);

    auto branchLengths = std::bind(&TreeParameter::getBranchLengths, &treeParam);
    ExponentialDistribution branchPrior(0.2, branchLengths);

    std::vector<AbstractMove*> moves {
        //&MoveTreeNNI(&treeParam)//,
        &MoveScaleBranch(&treeParam)
    };

    MoveScheduler moveScheduler(moves);

    Mcmc myMCMC(1000, 1, 1, 100, &model, &branchPrior, &moveScheduler);
    myMCMC.run(false);

    std::cout << treeParam.getTree()->getNewick() << std::endl;

}
