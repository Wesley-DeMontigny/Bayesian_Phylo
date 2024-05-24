#include <iostream>
#include <cmath>
#include <cassert>
#include "RandomVariable.hpp"
#include "Msg.hpp"
#include "Tree.hpp"
#include "ncl/ncl.h"

int main(int argc, char* argv[]) {

    RandomVariable& rv = RandomVariable::randomVariableInstance();

    Tree t1(&rv, 10);
    Tree t2(&rv, 20);
    std::cout << "Original T1:\n" << t1.getNewick() << std::endl;

    t1 = t2;
    std::cout << "Changed T1:\n" << t1.getNewick() << std::endl;
}
