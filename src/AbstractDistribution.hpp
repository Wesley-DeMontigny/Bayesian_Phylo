#ifndef ABSTRACT_DISTRIBUTION_HPP
#define ABSTRACT_DISTRIBUTION_HPP


class AbstractDistribution{
    public:
        virtual double lnLikelihood()=0;
    private:
};

#endif