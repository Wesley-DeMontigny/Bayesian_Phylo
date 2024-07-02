#include "DoubleMatrix.hpp"

void DoubleMatrix::initializeMatrix(int r, int c){
    vals = new double[r*c];
    for(int i = 0; i < r*c; i++)
        vals[i] = 0.0;
}

DoubleMatrix::DoubleMatrix(int r, int c) : nrows(r), ncols(c){
    initializeMatrix(r, c);
}

DoubleMatrix::DoubleMatrix(int n) : nrows(n), ncols(n){
    initializeMatrix(n, n);
}

DoubleMatrix::DoubleMatrix(const DoubleMatrix& m) : nrows(m.nrows), ncols(m.ncols){
    vals = new double[nrows*ncols];
    for(int i = 0; i < nrows*ncols; i++)
        this->vals[i] = m.vals[i];
}

DoubleMatrix::~DoubleMatrix(){
    delete [] vals;
}

double& DoubleMatrix::operator()(size_t r, size_t c) {
    return this->vals[r*ncols+c];
}

const double& DoubleMatrix::operator()(size_t r, size_t c) const {
    return this->vals[r*ncols+c];
}

std::ostream& operator<<(std::ostream& out, const DoubleMatrix& m){
    out << "[";
    for(int i = 0; i < m.nrows; i++){
        out << "[";
        for(int j = 0; j < m.ncols; j++){
            out << m(i, j);
            if(j != m.ncols-1)
                out << ", ";
        }
        out << "]";
        if(i != m.nrows-1)
            out << ", ";
    }
    out << "]";
    return out;
}

DoubleMatrix& DoubleMatrix::operator=(const DoubleMatrix& rhs){
    if(this->nrows != rhs.nrows || this->ncols != rhs.ncols){
        delete [] vals;
        this->nrows = rhs.nrows;
        this->ncols = rhs.ncols;
        initializeMatrix(nrows, ncols);
    }
    memcpy(this->vals, rhs.vals, nrows*ncols*sizeof(double));
    return *this;
}