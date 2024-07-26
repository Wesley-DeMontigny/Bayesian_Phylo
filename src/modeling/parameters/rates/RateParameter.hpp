#ifndef RATE_PARAMETER_HPP
#define RATE_PARAMETER_HPP
#include "modeling/parameters/DoubleParameter.hpp"

class RateMatrix;

class RateParameter : public DoubleParameter {
    public:
        RateParameter(void);
        RateParameter(double v);
        void accept() {oldValue = currentValue;}
        void reject() {currentValue = oldValue;}
        void regenerate();
        double getValue() {return currentValue;}
        double setValue(double v) {currentValue = v;}
        std::string writeValue() {return std::to_string(currentValue);}
    protected:
        double currentValue;
        double oldValue;
};

#endif