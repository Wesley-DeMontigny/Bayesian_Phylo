#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "TreeParameter.hpp"
#include "MoveScheduler.hpp"
#include "MoveTreeNNI.hpp"
#include "MoveScaleBranch.hpp"
#include "PhyloCTMC.hpp"
#include "Mcmc.hpp"
#include "HillClimb.hpp"
#include "TreePrior.hpp"
#include "EventManager.hpp"
#include "TuneEvent.hpp"
#include "McmcFileLogEvent.hpp"
#include "McmcScreenLogEvent.hpp"
#include "IterationTrackerEvent.hpp"
#include "MoveTreeLocal.hpp"
#include "JC69Matrix.hpp"

int main(int argc, char* argv[]) {

    RandomVariable& rng = RandomVariable::randomVariableInstance();
    MoveScheduler moveScheduler;

    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    JC69Matrix rateMatrix;

    TreeParameter treeParam = TreeParameter(&aln);
    TreePrior treePrior(&treeParam);
    treePrior.setExponentialBranchPrior(0.2);

    PhyloCTMC model(&aln, &treeParam, &rateMatrix);

    MoveTreeNNI nniMove(&treeParam);
    MoveScaleBranch scaleBranchMove(&treeParam);
    MoveTreeLocal localMove(&treeParam);
    moveScheduler.registerMove(&nniMove);
    moveScheduler.registerMove(&scaleBranchMove);
    moveScheduler.registerMove(&localMove);

    Mcmc myMCMC(&model, &treePrior, &moveScheduler);

    EventManager burnIn;
    burnIn.registerEvent(&TuneEvent(&moveScheduler), 100);
    burnIn.registerEvent(&IterationTrackerEvent(), 10);

    burnIn.initialize();
    myMCMC.run(5000, &burnIn);

    EventManager realRun;
    McmcScreenLogEvent screenLogger;
    screenLogger.setLikelihood(&model);
    screenLogger.setPrior(&treePrior);
    realRun.registerEvent(&screenLogger, 100);
    McmcFileLogEvent fileLogger;
    fileLogger.setLikelihood(&model);
    fileLogger.setPrior(&treePrior);
    fileLogger.setFile("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/test_mcmc.log");
    realRun.registerEvent(&fileLogger, 10);
    
    realRun.initialize();
    myMCMC.run(20000, &realRun);

    std::cout << treeParam.getTree()->getNewick() << std::endl;
}
