#ifndef double_matrix
#define double_matrix
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
    private:
        double* vals;
        int nrows;
        int ncols;
        void initializeMatrix(int r, int c);
};

#endif