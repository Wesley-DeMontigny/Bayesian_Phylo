#include "RateParameter.hpp"
#include "AbstractPrior.hpp"
#include "AbstractLikelihood.hpp"
#include "RateMatrix.hpp"

void RateParameter::regenerate(){
    prior->regeneratePrior();
    matrix->regenerate();
    matrix->setNeedsUpdate(true);
}