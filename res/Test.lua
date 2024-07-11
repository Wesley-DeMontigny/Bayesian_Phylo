local aln = Alignment("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex")

local startingNewick = "((((((Varecia variegata variegata:0.11306248,Lemur catta:0.08517853):0.03551525,Propithecus coquereli:0.08975919):0.02524162,(Microcebus murinus:0.12098289,Cheirogaleus major:0.07742627):0.03788273):0.03242533,Lepilemur hubbardorum:0.12369700):0.04809906,Daubentonia madagascariensis:0.12736299):0.02165753,(Tarsius syrichta:0.16880438,((Loris tardigradus:0.09706054,Nycticebus coucang:0.11094180):0.03326148,(Perodicticus potto:0.09688049,(Galago senegalensis:0.07841333,Otolemur crassicaudatus:0.08471203):0.04614413):0.02599167):0.03058266):0.02364311,(Galeopterus variegatus:0.16926509,((((Cebus albifrons:0.09819050,Aotus trivirgatus:0.10440388):0.02556585,Callicebus donacophilus:0.10178370):0.03397762,Saimiri sciureus:0.12518660):0.07646005,((Hylobates lar:0.11389293,Pan paniscus:0.08160923):0.03225153,(Colobus guereza:0.12575824,(Chlorocebus aethiops:0.07923215,Macaca mulatta:0.09833276):0.02706995):0.04458596):0.05315203):0.04657363):0.02675300);"

local treeParam = TreeParameter(TreeObject(startingNewick, aln:getTaxaNames()));
local prior = FlatPrior()
local ctmc = PhyloCTMC(aln, treeParam)

local moves = {
    MoveTreeNNI(treeParam),
    MoveScaleBranch(treeParam)
}
local moveScheduler = MoveScheduler(moves)

local myMcmc = Mcmc(100, 1, 1, ctmc, prior, moveScheduler)
myMcmc:run()

local endTree = treeParam:getValue()
print(endTree:getNewick())