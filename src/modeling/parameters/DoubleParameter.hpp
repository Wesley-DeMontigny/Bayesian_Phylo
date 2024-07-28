#ifndef DOUBLE_PARAMETER_HPP
#define DOUBLE_PARAMETER_HPP
#include "modeling/ModelNode.hpp"

class DoubleParameter : public ModelNode {
    public:
        DoubleParameter(void);
        DoubleParameter(double v);
        void accept() {oldValue = currentValue;}
        void reject() {currentValue = oldValue;}
        void regenerate() {};
        double getValue() {return currentValue;}
        void setValue(double v) {currentValue = v;}
        std::string writeValue() {return std::to_string(currentValue);}
    protected:
        double currentValue;
        double oldValue;
};

#endif