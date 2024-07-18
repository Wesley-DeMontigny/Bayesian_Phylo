#ifndef ABSTRACT_LIKELIHOOD_HPP
#define ABSTRACT_LIKELIHOOD_HPP


class AbstractLikelihood{
    public:
        virtual double lnLikelihood()=0;
        virtual void regenerateLikelihood()=0;
        virtual void acceptLikelihood()=0;
        virtual void rejectLikelihood()=0;
    protected:
        double oldLikelihood;
        double currentLikelihood;
};

#endif