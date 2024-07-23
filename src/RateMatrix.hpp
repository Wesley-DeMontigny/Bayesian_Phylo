#ifndef RATE_MATRIX_HPP
#define RATE_MATRIX_HPP
#include "AbstractParameter.hpp"

class DoubleMatrix;
class AbstractPrior;
class AbstractLikelihood;

class RateMatrix : public AbstractParameter {
    public:
        virtual DoubleMatrix P(double v)=0;
        virtual bool getNeedsUpdate()=0;
        virtual void setNeedsUpdate(bool b)=0;
        virtual void accept()=0;
        virtual void reject()=0;
        virtual void setPrior(AbstractPrior* p)=0;
        virtual void setLikelihood(AbstractLikelihood* l)=0;
        virtual void regenerate()=0;
        virtual void initialize()=0;//Eigen value stuff.
        virtual std::string writeValue()=0;
    protected:
        AbstractPrior* prior;
        AbstractLikelihood* likelihood;
    private:
        bool needsUpdate;
};

#endif