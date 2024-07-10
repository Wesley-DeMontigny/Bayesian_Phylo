print("I am using Lua to do this MCMC!")

local aln = Alignment("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex")

local ctmc = PhyloCTMC(aln)
local myMcmc = Mcmc(100, 1, 1, ctmc)
myMcmc:run()