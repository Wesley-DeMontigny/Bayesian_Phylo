#include "RateParameter.hpp"
#include "RateMatrix.hpp"

RateParameter::RateParameter(double v) : currentValue(v), oldValue(v) {
    dirty();
}

void RateParameter::regenerate() {}