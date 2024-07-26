#include "JC69Matrix.hpp"
#include "core/DoubleMatrix.hpp"

JC69Matrix::JC69Matrix() {
    dirty();
}

//This will actually be important when we need to recalculate eigen values
void JC69Matrix::regenerate() {}

DoubleMatrix JC69Matrix::P(double v) {
    DoubleMatrix pMatrix(4);

    double expX = std::exp((-4.0/3.0) * v);
    double p0 = 0.25 + (0.75 * expX);
    double p1 = 0.25 - (0.25 * expX);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(i == j)
                pMatrix(i, j) = p0;
            else
                pMatrix(i, j) = p1;
        }
    }

    return pMatrix;
}