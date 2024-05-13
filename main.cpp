#include <iostream>
#include <cmath>
#include <cassert>
#include "RandomVariable.hpp"
#include "Msg.hpp"
#include "Tree.hpp"

int main(int argc, char* argv[]){
    RandomVariable& rv = RandomVariable::randomVariableInstance();

    Tree myTree(&rv, 10);
    myTree.print();

}