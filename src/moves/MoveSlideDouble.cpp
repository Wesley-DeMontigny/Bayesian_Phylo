#include "MoveSlideDouble.hpp"
#include "modeling/parameters/DoubleParameter.hpp"
#include "core/RandomVariable.hpp"
#include <iostream>
#include <cmath>

MoveSlideDouble::MoveSlideDouble(DoubleParameter* t, double w) : param(t), window(w) {}
        
double MoveSlideDouble::update(){

    RandomVariable& rng = RandomVariable::randomVariableInstance();

    double shift = window * (rng.uniformRv() - 0.5);
    double newV = param->getValue() + shift;

    param->setValue(newV);
    param->dirty();

    return 0.0;
}

void MoveSlideDouble::tune(){
    acceptedSinceTune = 0;
    countSinceTune = 0;
}