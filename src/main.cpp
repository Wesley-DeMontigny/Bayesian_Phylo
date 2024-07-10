#include <iostream>
#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "Tree.hpp"
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
        .beginClass<Tree>("Tree")
            .addFunction("getNewick", &Tree::getNewick)
        .endClass()
        .beginClass<PhyloCTMC>("PhyloCTMC")
            .addConstructor<void(Alignment*)>()
            .addFunction("getTree", &PhyloCTMC::getActiveTree)
        .endClass()
        .beginClass<Mcmc>("Mcmc")
            .addConstructor<void(int, int, int, PhyloCTMC*)>()
            .addFunction("run", &Mcmc::run)
        .endClass();

    const char* script = "C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/Test.lua";

    if (luaL_dofile(L, script)) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);

    /*
    Alignment aln("C:/Users/wescd/OneDrive/Documents/Code/Bayesian_Phylo/Bayesian_Phylo/res/primates_and_galeopterus_cytb.nex");

    PhyloCTMC model(&aln);

    Mcmc myMCMC(100, 1, 1, &model);
    myMCMC.run();
    */
}
