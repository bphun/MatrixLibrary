/**
 * This is a basic implementation of a matrix class which contains some commonly used
 * operations such as scalar addition, matrix addition, scalar subtraction, matrix subraction,
 * scalar multiplication, matrix multiplication, and scalar division. This library is still in development
 * so new features such as determinant calculation, matrix inverse computation, matrix division, OpenCL 
 * GPU acceleration, and CUDA GPU acceleration will be added in the near future. Any feature suggestions
 * and bug reports and be submitted to this project's issue page on Github. 
 */

#ifndef MATRIX_H
#define MATRIX_H

using namespace std;

template <typename T>
class Matrix
{
    static_assert(std::is_arithmetic<T>::value, "typename T must be a numeric type");

private:
    size_t numRows, numCols;

    T *mat;

public:
    Matrix();
    Matrix(Matrix<T> &);
    Matrix(size_t, size_t);
    Matrix(T *, size_t, size_t);
    Matrix(vector<T>, size_t, size_t);

    ~Matrix();

    size_t cols() const;
    size_t rows() const;
    int size() const;

    T *flatten();
    T elementAt(int, int) const;

    void print();
    void setElementAt(int, int, T);
    void inverse();

    float determinant() const;

    Matrix<T> &operator+(Matrix<T> &);
    Matrix<T> &operator-(Matrix<T> &);
    Matrix<T> &operator*(T);
    Matrix<T> &operator*(Matrix<T> &);
    Matrix<T> &operator/(Matrix<T> &);
    Matrix<T> &operator/(T scalar);

    bool equals(Matrix<T> &);
};

#ifdef OPENCL
#include <OpenCL/cl.h>
#include "Matrix_opencl.cpp"
#else
#include "Matrix_cpu.cpp"
#endif

#endif