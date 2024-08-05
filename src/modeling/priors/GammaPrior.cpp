#define _USE_MATH_DEFINES
#include "GammaPrior.hpp"
#include "modeling/parameters/DoubleParameter.hpp"
#include "core/RandomVariable.hpp"
#include <cmath>
#include <iostream>

GammaPrior::GammaPrior(DoubleParameter* a, DoubleParameter* b, DoubleParameter* p) : currentLnPrior(0.0), oldLnPrior(0.0), shape(a), rate(b), param(p) {
    this->dirty();
}

void GammaPrior::accept() {
    oldLnPrior = currentLnPrior;

    shape->accept();
    shape->clean();
    rate->accept();
    rate->clean();
    param->accept();
    param->clean();
}

void GammaPrior::reject() {
    currentLnPrior = oldLnPrior;

    shape->reject();
    shape->clean();
    rate->reject();
    rate->clean();
    param->reject();
    param->clean();
}

void GammaPrior::regenerate(){
    shape->regenerate();
    rate->regenerate();
    param->regenerate();

    if(shape->isDirty() || rate->isDirty() || param->isDirty())
            this->dirty();
    

    if(this->isDirty()){
        double shapeVal = shape->getValue();
        double rateVal = rate->getValue();
        double val = param->getValue();

        double d1 = std::pow(rateVal, shapeVal) / std::tgamma(shapeVal);
        double d2 = std::pow(val, shapeVal-1.0) * std::pow(M_E, -1.0 * rateVal * val);

        currentLnPrior = d1 * d2;

    }
}

void GammaPrior::sample(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    double shapeVal = shape->getValue();
    double rateVal = rate->getValue();
    param->setValue(rng.gammaRv(shapeVal, rateVal));

    param->accept();
    param->regenerate();
}