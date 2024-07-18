#ifndef ABSTRACT_PARAMETER_HPP
#define ABSTRACT_PARAMETER_HPP
#include <string>

class AbstractPrior;
class AbstractLikelihood;


class AbstractParameter{
    public:
        virtual void accept()=0;
        virtual void reject()=0;
        virtual void setPrior(AbstractPrior* p)=0;
        virtual void setLikelihood(AbstractLikelihood* l)=0;
        virtual void regenerate()=0;
        virtual std::string writeValue()=0;
    protected:
        AbstractPrior* prior;
        AbstractLikelihood* likelihood;
};

#endif