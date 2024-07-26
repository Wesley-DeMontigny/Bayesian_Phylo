#include "DoubleParameter.hpp"

DoubleParameter::DoubleParameter(void) : currentValue(0.0), oldValue(0.0) {}

DoubleParameter::DoubleParameter(double v) : currentValue(v), oldValue(v) {}
