#include "MoveScaleDouble.hpp"
#include "DoubleParameter.hpp"
#include "TreeObject.hpp"
#include "RandomVariable.hpp"
#include "Node.hpp"
#include <iostream>
#include <cmath>

MoveScaleDouble::MoveScaleDouble(DoubleParameter* t) : param(t), delta(0.25) {}
        
double MoveScaleDouble::update(){

    RandomVariable& rng = RandomVariable::randomVariableInstance();

    double scale = std::exp(delta * (rng.uniformRv() - 0.5));
    double newV = param->getValue() * scale;

    param->setValue(newV);
    param->regenerate();

    return scale;
}

void MoveScaleDouble::accept(){
    param->accept();
    acceptedCount++;
    acceptedSinceTune++;
    countSinceTune++;
}

void MoveScaleDouble::reject(){
    param->reject();
    rejectedCount++;
    countSinceTune++;
}

void MoveScaleDouble::tune(){
    double rate = (double)acceptedSinceTune/(double)countSinceTune;

    if ( rate > 0.44 ) {
        delta *= (1.0 + ((rate-0.44)/0.766));
    }
    else {
        delta /= (2.0 - rate/0.44);
    }

    acceptedSinceTune = 0;
    countSinceTune = 0;
}