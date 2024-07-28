#ifndef JC69_MATRIX_HPP
#define JC69_MATRIX_HPP
#include "RateMatrix.hpp"

class DoubleMatrix;

class JC69Matrix : public RateMatrix {
    public:
        JC69Matrix(void);
        DoubleMatrix P(double v);
        void accept() {};
        void reject() {};
        void regenerate();
        std::string writeValue() {return "";}//There are no parameters to print...
};

#endif