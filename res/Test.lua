print("I am using Lua to do this MCMC!")

local aln = Alignment("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex")

local ctmc = PhyloCTMC(aln)
local starterTree = ctmc:getTree()
print(starterTree:getNewick())

local myMcmc = Mcmc(10000, 1, 1, ctmc)
myMcmc:run()

local endTree = ctmc:getTree()
print(endTree:getNewick())