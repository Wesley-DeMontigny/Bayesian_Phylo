#ifndef JC69_MATRIX_HPP
#define JC69_MATRIX_HPP
#include "RateMatrix.hpp"

class DoubleMatrix;

class JC69Matrix : public RateMatrix {
    public:
        JC69Matrix(void);
        DoubleMatrix P(double v);
        bool getNeedsUpdate() {return needsUpdate;}
        void setNeedsUpdate(bool b) {needsUpdate = true;}
        void accept() {}; //Nothing to accept
        void reject() {}; //Nothing to reject
        void setPrior(AbstractPrior* p) {prior = p;}
        void setLikelihood(AbstractLikelihood* l) {likelihood = l;}
        void regenerate();
        void initialize();
        std::string writeValue() {return "";}//There are no parameters to print...
    protected:
        AbstractPrior* prior;
        AbstractLikelihood* likelihood;
    private:
        bool needsUpdate;
};

#endif