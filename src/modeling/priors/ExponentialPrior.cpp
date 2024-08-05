#include "ExponentialPrior.hpp"
#include "modeling/parameters/DoubleParameter.hpp"
#include "core/RandomVariable.hpp"
#include <cmath>
#include <iostream>

ExponentialPrior::ExponentialPrior(DoubleParameter* l, DoubleParameter* p) : currentLnPrior(0.0), oldLnPrior(0.0), lambda(l), param(p) {
    this->dirty();
}

void ExponentialPrior::accept() {
    oldLnPrior = currentLnPrior;

    lambda->accept();
    lambda->clean();
    param->accept();
    param->clean();
}

void ExponentialPrior::reject() {
    currentLnPrior = oldLnPrior;

    lambda->reject();
    lambda->clean();
    param->reject();
    param->clean();
}

void ExponentialPrior::regenerate(){
    lambda->regenerate();
    param->regenerate();

    if(lambda->isDirty() || param->isDirty())
            this->dirty();
    

    if(this->isDirty()){
        double lambdaVal = lambda->getValue();
        double val = param->getValue();
        if(val > 0)
            currentLnPrior = std::log(lambdaVal * std::exp(-1 * (lambdaVal) * (val)));
        else
            currentLnPrior = -INFINITY;
    }
}

void ExponentialPrior::sample(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    double lambdaVal = lambda->getValue();
    param->setValue(rng.exponentialRv(lambdaVal));

    param->accept();
    param->regenerate();
}