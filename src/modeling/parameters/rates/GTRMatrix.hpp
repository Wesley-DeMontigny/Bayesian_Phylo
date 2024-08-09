#ifndef GTR_MATRIX_HPP
#define GTR_MATRIX_HPP
#include "RateMatrix.hpp"
#include "core/DoubleMatrix.hpp"
#include "modeling/parameters/DoubleParameter.hpp"

class GTRMatrix : public RateMatrix {
    public:
        GTRMatrix(void)=delete;
        GTRMatrix(DoubleParameter** r);
        DoubleMatrix P(double v);
        void accept();
        void reject();
        void regenerate();
        std::string writeValue() {return "";}
    private:
        DoubleMatrix currentPMatrix;
        DoubleMatrix oldPMatrix;
        DoubleParameter** rates;
};

#endif