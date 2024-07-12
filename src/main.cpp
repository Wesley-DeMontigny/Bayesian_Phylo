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
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/Vector.h"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();

    /*
    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    TreeObject treeObj(&aln);
    TreeParameter treeParam = TreeParameter(&treeObj);
    PhyloCTMC model(&aln, &treeParam);
    FlatPrior priorD;

    std::vector<AbstractMove*> moves {
        &MoveTreeNNI(&treeParam), 
        &MoveScaleBranch(&treeParam)
    };
    MoveScheduler moveScheduler(moves);

    Mcmc myMCMC(100, 1, 1, &model, &priorD, &moveScheduler);
    myMCMC.run();
    */

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<Alignment>("Alignment")
            .addConstructor<void(std::string)>()
            .addFunction("getTaxaNames", &Alignment::getTaxaNames)
        .endClass()
        .beginClass<TreeObject>("TreeObject")
            .addConstructor<void(int), 
            void(std::string, std::vector<std::string>),
            void(double, double, double)>()
            .addFunction("getNewick", &TreeObject::getNewick)
            .addFunction("getNumTaxa", &TreeObject::getNumTaxa)
        .endClass()
        .beginClass<TreeParameter>("TreeParameter")
            .addConstructor<void(TreeObject*)>()
            .addFunction("getValue", &TreeParameter::getValue)
        .endClass()
        .beginClass<AbstractMove>("AbstractMove")
        .endClass()
        .deriveClass<MoveTreeNNI, AbstractMove>("MoveTreeNNI")
            .addConstructor<void(TreeParameter*)>()
        .endClass()
        .deriveClass<MoveScaleBranch, AbstractMove>("MoveScaleBranch")
            .addConstructor<void(TreeParameter*)>()
        .endClass()
        .beginClass<MoveScheduler>("MoveScheduler")
            .addConstructor<void(std::vector<AbstractMove*>)>()
            .addFunction("registerMove", &MoveScheduler::registerMove)
        .endClass()
        .beginClass<AbstractDistribution>("AbstractDistribution")
        .endClass()
        .deriveClass<PhyloCTMC, AbstractDistribution>("PhyloCTMC")
            .addConstructor<void(Alignment*, TreeParameter*)>()
            .addFunction("lnLikelihood", &PhyloCTMC::lnLikelihood)
        .endClass()
        .deriveClass<FlatPrior, AbstractDistribution>("FlatPrior")
            .addConstructor<void(void)>()
        .endClass()
        .beginClass<Mcmc>("Mcmc")
            .addConstructor<void(int, int, int, AbstractDistribution*, AbstractDistribution*, MoveScheduler*)>()
            .addFunction("run", &Mcmc::run)
        .endClass();

    const char* script = "C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/Test.lua";

    if (luaL_dofile(L, script)) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);
}
