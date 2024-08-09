#ifndef LG_MATRIX_HPP
#define LG_MATRIX_HPP
#include "RateMatrix.hpp"
#include "core/DoubleMatrix.hpp"
#include "modeling/parameters/DoubleParameter.hpp"

class LGMatrix : public RateMatrix {
    public:
        LGMatrix(void);
        DoubleMatrix P(double v);
        void accept();
        void reject();
        void regenerate();
        std::string writeValue() {return "";}
    private:
        DoubleMatrix PMatrix;
};

#endif