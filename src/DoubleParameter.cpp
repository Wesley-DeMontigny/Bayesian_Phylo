#include "DoubleParameter.hpp"
#include "AbstractPrior.hpp"
#include "AbstractLikelihood.hpp"

DoubleParameter::DoubleParameter(void) : currentValue(0.0), oldValue(0.0), likelihood(nullptr), prior(nullptr) {}

DoubleParameter::DoubleParameter(double v) : currentValue(v), oldValue(v), likelihood(nullptr), prior(nullptr) {}

void DoubleParameter::regenerate() {
    if(prior != nullptr)
        prior->regeneratePrior();
    if(likelihood != nullptr)
        likelihood->regenerateLikelihood();
}