#ifndef DOUBLE_MATRIX_HPP
#define DOUBLE_MATRIX_HPP
#include <iostream>

class DoubleMatrix{
    public:
        DoubleMatrix(void) = delete;
        DoubleMatrix(int n);
        DoubleMatrix(int r, int c);
        DoubleMatrix(const DoubleMatrix& m);
        ~DoubleMatrix();
        double& operator()(size_t r, size_t c);
        const double& operator()(size_t r, size_t c) const;
        friend std::ostream& operator<<(std::ostream& out, const DoubleMatrix& m);
        DoubleMatrix& operator=(const DoubleMatrix& rhs);
        int getCols(){return ncols;}
        int getRows(){return nrows;}
    private:
        void initializeMatrix(int r, int c);
        int nrows;
        int ncols;
        double* vals;
};

#endif