#define _USE_MATH_DEFINES
#include "NormalPrior.hpp"
#include "modeling/parameters/DoubleParameter.hpp"
#include "core/RandomVariable.hpp"
#include <cmath>
#include <iostream>

NormalPrior::NormalPrior(DoubleParameter* m, DoubleParameter* s, DoubleParameter* p) : currentLnPrior(0.0), oldLnPrior(0.0), mu(m), sigma(s), param(p) {
    this->dirty();
}

void NormalPrior::accept() {
    oldLnPrior = currentLnPrior;

    mu->accept();
    mu->clean();
    sigma->accept();
    sigma->clean();
    param->accept();
    param->clean();
}

void NormalPrior::reject() {
    currentLnPrior = oldLnPrior;

    mu->reject();
    mu->clean();
    sigma->reject();
    sigma->clean();
    param->reject();
    param->clean();
}

void NormalPrior::regenerate(){
    mu->regenerate();
    sigma->regenerate();
    param->regenerate();

    if(mu->isDirty() || sigma->isDirty() || param->isDirty())
            this->dirty();
    

    if(this->isDirty()){
        double sigmaVal = sigma->getValue();
        double muVal = mu->getValue();
        double val = param->getValue();

        double sigma2 = sigmaVal * sigmaVal;
        double a = 1.0 / (std::sqrt(2.0 * M_PI * sigma2)) * M_E;
        double b = -1.0 * std::pow(val - muVal, 2) / (2 * sigma2);
        currentLnPrior = std::pow(a, b);
    }
}

void NormalPrior::sample(){
    RandomVariable& rng = RandomVariable::randomVariableInstance();
    double sigmaVal = sigma->getValue();
    double muVal = mu->getValue();
    param->setValue(rng.normalRv(muVal, sigmaVal));

    param->accept();
    param->regenerate();
}