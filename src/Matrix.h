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

#include <stdio.h>
#include <vector>

#ifdef OPENCL_ENABLE
#include <fstream>
#if __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include "GpuCompute.h"
#endif

using namespace std;

template <typename T>
class Matrix
{
    static_assert(std::is_arithmetic<T>::value, "typename T must be a numeric type");

private:
    size_t numRows, numCols;

    T *mat;

public:
    /**
     * Initialize an empty matrix with 0 rows and 0 columns
     */
    Matrix();

    /**
     * Initialize a matrix with the same contents as the specified matrix.
     * The new matrix will also have the same number of rows and colums as the speciifed matrix.
     * 
     * @param matrix The matrix that will be copied to the new matrix
     */
    Matrix(Matrix<T> &);

    /**
     * Initialize a matrix filled with zeros with the specified
     * number of rows and columns
     * 
     * @param rows Number of rows that the matrix will have
     * @param cols Number of columns that the matrix will have
     */
    Matrix(size_t, size_t);

    /**
     * Initialize a matrix with values that are in an array of a numerical generic T.
     * This matrix will have the same number of rows and columns that
     * are specified by the user in the rows and cols parameters.
     * 
     * @param arr A pointer array of numerical generic T which will be loaded into the matrix
     * @param rows Number of rows that the matrix will have
     * @param cols Number of columns that the matrix will have
     */
    Matrix(T *, size_t, size_t);

    /**
     * Initialize a matrix with values that are in a vector of a numerical generic T.
     * This matrix will have the same number of rows and columns that
     * are specified by the user in the rows and cols parameters.
     * 
     * @param arr A vector with numerical generic T which will be loaded into the matrix
     * @param rows Number of rows that the matrix will have
     * @param cols Number of columns that the matrix will have
     */
    Matrix(vector<T>, size_t, size_t);

    /**
     * Cleanup matrix and associated processes 
     */
    ~Matrix();

    /**
     * @return Returns the number of colums that are in the matrix
     */
    size_t cols() const;

    /**
     * @return Returns the number of rows that are in the matrix
     */
    size_t rows() const;

    /**
     * @return Returns the total number of elements in the matrix by multiplying
     * the number of rows by the number of columns.
     */
    int size() const;

    /**
     * @return A flattened version of the matrix.
     * For example, a matrix that looks like this:
     *        1 2
     *        3 4
     * will be converted into an array that looks like this:
     *      1 2 3 4
     * 
     */
    T *flatten();

    /**
     * Since the matrix is stored a a one-dimensional pointer array, we must do some 
     * math to grab a value at a row and column. The index of the element is computed by simply 
     * multiplying the row of the desired element by the total number of columns in the matrix
     * and then adding the column of the desired element. Or simply put, (row * numCols + col)
     * In the future, this function will be replaced with a [] operator overload.
     * 
     * @param row The row of the desired element
     * @param col
     * 
     * @return The element at the specified row and column
     */
    T elementAt(int, int) const;

    /**
     * Prints a representation of the matrix to stdout
     */
    void print();

    /**
     * Sets the element at the specified row and column to the desired value.
     * 
     * @param row Row where the specified value will be placed
     * @param col Column where the specified value will be placed
     * @param value The value with type T, a generic number, that will be placed at the specified row and column
     */
    void setElementAt(int, int, T);

    /**
     * Converts a matrix to its inverse 
     */
    void inverse();

    /**
     * Calculates the determinant of a matrix
     * 
     * @return Floating point value representing the determinant of the matrix
     */
    float determinant() const;

    /**
     * Adds the contents of two matrices
     * 
     * @param matrix The matrix that will be combined with the contents of the calling matrix
     * 
     * @return A matrix that contains the values of the combined matrices 
     */
    Matrix<T> &operator+(Matrix<T> &);

    /**
     * Subtracts the contents of two matrices
     * 
     * @param matrix The matrix that will be subtracted from with the contents of the calling matrix
     * 
     * @return A matrix that contains the values of the subtracted matrices 
     */
    Matrix<T> &operator-(Matrix<T> &);

    /**
     * Multiplies the contents a matrix by a scalar constant
     * 
     * @param scalar The scalar that will be added to each element in the calling matrix
     * 
     * @return A matrix where element has increased by the value specified in scalar
     */
    Matrix<T> &operator*(T);

    /**
     * Multiplies two matrices with each other
     * 
     * @param matrix Matrix that will be muliplied with the calling matrix
     * 
     * @return A matrix which contains the result of the multiplication of two matrices
     */
    Matrix<T> &operator*(Matrix<T> &);

    /**
     * Divide two matrices
     * 
     * @param matrix Matrix that will be divided by the calling matrix
     * 
     * @return A matrix which contains the result of the divsion of the two matrices
     */
    Matrix<T> &operator/(Matrix<T> &);

    /**
     * Divides the contents of a matrix by a scalar value
     * 
     * @param The scalar quantity by which all the contents in the calling matrix will be dividied by
     * 
     * @return A matrix which contains the values of the calling matrix divided by the scalar quantity
     */
    Matrix<T> &operator/(T);

    /**
     * Determines if two matrices contain the same elements
    * 
    * @return Returns true if the contents of the two matrices are equal and false if otherwise
    */
    bool equals(Matrix<T> &);
};

#ifdef OPENCL_ENABLE
#include "Matrix_opencl.cpp"
#else
#include "Matrix_cpu.cpp"
#endif
#endif