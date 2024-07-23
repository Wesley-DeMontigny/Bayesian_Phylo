#ifndef RATE_PARAMETER_HPP
#define RATE_PARAMETER_HPP
#include "DoubleParameter.hpp"

class RateMatrix;

class RateParameter : public DoubleParameter {
    public:
        RateParameter(void);
        RateParameter(double v);
        void accept() {oldValue = currentValue;}
        void reject() {currentValue = oldValue;}
        void setPrior(AbstractPrior* p) {prior = p;}
        void setLikelihood(AbstractLikelihood* l) {likelihood = l;}
        void setRateMatrix(RateMatrix* m);
        RateMatrix* getRateMatrix() {return matrix;}
        void regenerate();
        double getValue() {return currentValue;}
        double setValue(double v) {currentValue = v;}
        std::string writeValue() {return std::to_string(currentValue);}
    protected:
        double currentValue;
        double oldValue;
        RateMatrix* matrix;
        AbstractPrior* prior;
        AbstractLikelihood* likelihood;
};

#endif