#include "core/RandomVariable.hpp"
#include "core/Alignment.hpp"
#include "moves/MoveScheduler.hpp"
#include "moves/MoveTreeNNI.hpp"
#include "moves/MoveScaleBranch.hpp"
#include "moves/Move.hpp"
#include "moves/MoveTreeLocal.hpp"
#include "moves/MoveScaleClade.hpp"
#include "moves/MoveSlideDouble.hpp"
#include "events/EventManager.hpp"
#include "events/TuneEvent.hpp"
#include "events/FileLogEvent.hpp"
#include "events/ScreenLogEvent.hpp"
#include "events/IterationTrackerEvent.hpp"
#include "modeling/parameters/trees/TreeParameter.hpp"
#include "modeling/parameters/rates/JC69Matrix.hpp"
#include "modeling/parameters/DoubleParameter.hpp"
#include "modeling/likelihoods/PhyloCTMC.hpp"
#include "modeling/priors/TreePrior.hpp"
#include "modeling/PosteriorNode.hpp"
#include "modeling/analysis/Mcmc.hpp"
#include "modeling/analysis/PerturbedHillClimb.hpp"
#include "modeling/priors/GammaPrior.hpp"


int main(int argc, char* argv[]) {

    RandomVariable& rng = RandomVariable::randomVariableInstance();
    MoveScheduler moveScheduler;

    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    JC69Matrix rateMatrix;

    DoubleParameter lambda(20.0);

    TreeParameter treeParam(&aln);
    TreePrior treePrior(&treeParam);
    treePrior.setExponentialBranchPrior(&lambda);
    treePrior.sample();

    PhyloCTMC ctmc(&aln, &treeParam, &rateMatrix);

    MoveTreeNNI nniMove(&treeParam);
    MoveScaleBranch scaleBranch(&treeParam);
    MoveTreeLocal localMove(&treeParam);
    moveScheduler.registerMove(&nniMove, 10);
    moveScheduler.registerMove(&scaleBranch, 3.0);
    moveScheduler.registerMove(&localMove, 5.0);

    PosteriorNode posterior(&ctmc, {&treePrior});

    Mcmc myMCMC(&posterior, &moveScheduler);

    EventManager burnIn;
    burnIn.registerEvent(&TuneEvent(&moveScheduler), 500);
    burnIn.registerEvent(&IterationTrackerEvent(), 10);
    burnIn.initialize();

    myMCMC.run(5000, &burnIn);

    std::vector<std::pair<std::string, ModelNode*>> loggables;
    loggables.push_back(std::make_pair("Prior", &treePrior));
    loggables.push_back(std::make_pair("Likelihood", &ctmc));
    loggables.push_back(std::make_pair("Posterior", &posterior));

    EventManager realRun;
    ScreenLogEvent screenLogger(loggables);
    realRun.registerEvent(&screenLogger, 100);
    FileLogEvent fileLogger(loggables, "C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/test_mcmc.log");
    realRun.registerEvent(&fileLogger, 10);
    realRun.initialize();

    myMCMC.run(30000, &realRun);

    std::cout << treeParam.getTree()->getNewick() << std::endl;
}
