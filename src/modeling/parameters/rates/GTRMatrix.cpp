#include "GTRMatrix.hpp"

GTRMatrix::GTRMatrix(DoubleParameter** r) : oldPMatrix(4), currentPMatrix(4), rates(r) {
    dirty();
}

void GTRMatrix::accept() {
    for(int i = 0; i < 6; i++){
        rates[i]->accept();
        rates[i]->clean();
    }

    oldPMatrix = currentPMatrix;
}

void GTRMatrix::reject() {
    for(int i = 0; i < 6; i++){
        rates[i]->accept();
        rates[i]->clean();
    }

    currentPMatrix = oldPMatrix;
}

void GTRMatrix::regenerate() {
    for(int i = 0; i < 6; i++){
        rates[i]->regenerate();

        if(rates[i]->isDirty())
            this->dirty();
    }

    if(this->isDirty())
        return; //Exponentiate and save to currentPMatrix
}

DoubleMatrix GTRMatrix::P(double v) {
    DoubleMatrix p(4);

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            p(i, j) = 0.0; //Matrix exp using v and currentPMatrix here
        }
    }

    return p;
}