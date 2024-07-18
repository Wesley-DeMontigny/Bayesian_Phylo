#ifndef ABSTRACT_PRIOR_HPP
#define ABSTRACT_PRIOR_HPP


class AbstractPrior{
    public:
        virtual double lnPrior()=0;
        virtual void regeneratePrior()=0;
        virtual void acceptPrior()=0;
        virtual void rejectPrior()=0;
    protected:
        double oldPrior;
        double currentPrior;
};

#endif