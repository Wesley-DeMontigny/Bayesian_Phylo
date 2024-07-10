#ifndef ABSTRACT_DISTRIBUTION_HPP
#define ABSTRACT_DISTRIBUTION_HPP


class AbstractDistribution{
    public:
        virtual double lnLikelihood()=0;
        virtual double lnPrior()=0;
        virtual double update()=0;
        virtual void accept()=0;
        virtual void reject()=0;
    private:
};

#endif