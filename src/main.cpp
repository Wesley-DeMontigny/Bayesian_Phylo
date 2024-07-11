#include <iostream>
#include "RandomVariable.hpp"
#include "Alignment.hpp"
#include "Tree.hpp"
#include "PhyloCTMC.hpp"
#include "Mcmc.hpp"
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/Vector.h"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luabridge::getGlobalNamespace(L)
        .beginClass<Alignment>("Alignment")
            .addConstructor<void(std::string)>()
            .addFunction("getTaxaNames", &Alignment::getTaxaNames)
        .endClass()
        .beginClass<Tree>("Tree")
            .addConstructor<
            void(int), 
            void(std::string, std::vector<std::string>),
            void(double, double, double)
            >()
            .addFunction("getNewick", &Tree::getNewick)
            .addFunction("getNumTaxa", &Tree::getNumTaxa)
        .endClass()
        .beginClass<PhyloCTMC>("PhyloCTMC")
            .addConstructor<void(Alignment*), void(Alignment*, Tree*)>()
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
