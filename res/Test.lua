local aln = Alignment("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cox2.nex")

local treeParam = TreeParameter(23);
local prior = FlatPrior()
local ctmc = PhyloCTMC(aln, treeParam)

local moves = {
    MoveTreeNNI(treeParam),
    MoveScaleBranch(treeParam)
}
local moveScheduler = MoveScheduler(moves)

local myMcmc = Mcmc(20000, 1, 1, ctmc, prior, moveScheduler)
myMcmc:run()

local endTree = treeParam:getValue()
print(endTree:getNewick())