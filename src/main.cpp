#include <iostream>
#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "PhyloCTMC.hpp"
#include "Mcmc.hpp"
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<Alignment>("Alignment")
            .addConstructor<void(std::string)>()
        .endClass()
        .beginClass<PhyloCTMC>("PhyloCTMC")
            .addConstructor<void(Alignment*)>()
        .endClass()
        .beginClass<Mcmc>("Mcmc")
            .addConstructor<void(int, int, int, PhyloCTMC*)>()
            .addFunction("run", &Mcmc::run)
        .endClass();

    luaL_dofile(L, "C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/Test.lua");

    lua_close(L);

    /*
    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    PhyloCTMC model(&aln);

    Mcmc myMCMC(100, 1, 1, &model);
    myMCMC.run();
    */
}
