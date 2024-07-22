#ifndef DOUBLE_PARAMETER_HPP
#define DOUBLE_PARAMETER_HPP
#include "AbstractParameter.hpp"

class DoubleParameter : public AbstractParameter {
    public:
        DoubleParameter(void);
        DoubleParameter(double v);
        void accept() {oldValue = currentValue;}
        void reject() {currentValue = oldValue;}
        void setPrior(AbstractPrior* p) {prior = p;}
        void setLikelihood(AbstractLikelihood* l) {likelihood = l;}
        void regenerate();
        double getValue() {return currentValue;}
        void setValue(double v) {currentValue = v;}
        std::string writeValue() {return std::to_string(currentValue);}
    protected:
        double currentValue;
        double oldValue;
        AbstractPrior* prior;
        AbstractLikelihood* likelihood;
};

#endif