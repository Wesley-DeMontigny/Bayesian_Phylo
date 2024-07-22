#include "RateParameter.hpp"
#include "AbstractPrior.hpp"
#include "AbstractLikelihood.hpp"
#include "RateMatrix.hpp"

void RateParameter::regenerate(){
    prior->regeneratePrior();
    //likelihood->regenerateLikelihood(); 
    //I don't think we should let the individual rates regenerate the likelihood - just the matrix.
    matrix->regenerate();
    matrix->setNeedsUpdate(true);
}