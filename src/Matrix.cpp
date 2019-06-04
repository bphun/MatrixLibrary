#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix()
{
    computeMethod.initialize();

    this->numRows = 0;
    this->numCols = 0;

    mat = new T[0];
}

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols)
{
    computeMethod.initialize();

    this->numRows = rows;
    this->numCols = cols;

    mat = new T[size()];

    for (int i = 0; i < size(); i++)
        mat[i] = 0;
}

template <typename T>
Matrix<T>::Matrix(T *arr, size_t rows, size_t cols)
{
    computeMethod.initialize();

    this->numRows = rows;
    this->numCols = cols;

    mat = new T[size()];

    for (int i = 0; i < size(); i++)
        setElementAt(i / cols, i % cols, arr[i]);
}

template <typename T>
Matrix<T>::Matrix(vector<T> arr, size_t rows, size_t cols)
{
    computeMethod.initialize();

    this->numRows = rows;
    this->numCols = cols;

    mat = new T[size()];

    for (int i = 0; i < size(); i++)
        setElementAt(i / cols, i % cols, arr[i]);
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> &matrix)
{
    computeMethod.initialize();

    this->numRows = matrix.rows();
    this->numCols = matrix.cols();

    mat = new T[matrix.size()];

    memcpy(mat, matrix.flatten(), sizeof(T *));
}

template <typename T>
Matrix<T>::~Matrix()
{
    delete[] mat;

    computeMethod.deinitialize();
}

template <typename T>
size_t Matrix<T>::cols() const
{
    return this->numCols;
}

template <typename T>
size_t Matrix<T>::rows() const
{
    return this->numRows;
}

template <typename T>
int Matrix<T>::size() const
{
    return numRows * numCols;
}

template <typename T>
T Matrix<T>::elementAt(int row, int col) const
{
    return mat[(row * numCols) + col];
}

template <typename T>
void Matrix<T>::setElementAt(int row, int col, T value)
{
    mat[(row * numCols) + col] = value;
}

template <typename T>
float Matrix<T>::determinant() const
{
}

template <typename T>
T *Matrix<T>::flatten()
{
    return mat;
}

template <typename T>
void Matrix<T>::inverse()
{
}

template <typename T>
void Matrix<T>::print()
{
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
            cout << elementAt(r, c) << " ";
        cout << endl;
    }
}

template <typename T>
Matrix<T> &Matrix<T>::operator+(Matrix<T> &matrix)
{
    if (numRows <= 0 || matrix.rows() <= 0 || numCols <= 0 || matrix.cols() <= 0)
        throw invalid_argument("Matrix addition requires non-empty matrices");

    if (numRows != matrix.rows() || numCols != matrix.cols())
        throw invalid_argument("Matrix addition requires two matrices of equal shape");

    T *resultMatrix = new T[size()];
    T *inputMatrix = matrix.flatten();
    size_t memSize = size() * sizeof(*mat);

    computeMethod.doMatrixAddition(mat, inputMatrix, resultMatrix, numCols, size());

    memcpy(mat, resultMatrix, memSize);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-(Matrix<T> &matrix)
{
    if (numRows <= 0 || matrix.rows() <= 0 || numCols <= 0 || matrix.cols() <= 0)
        throw invalid_argument("Matrix subtraction requires non-empty matrices");

    if (numRows != matrix.rows() || numCols != matrix.cols())
        throw invalid_argument("Matrix subtraction requires two matrices of equal shape");

    T *resultMatrix = new T[size()];
    T *inputMatrix = matrix.flatten();
    size_t memSize = size() * sizeof(*mat);

    computeMethod.doMatrixSubtraction(mat, inputMatrix, resultMatrix, numCols, size());

    memcpy(mat, resultMatrix, memSize);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*(T scalar)
{
    if (numRows <= 0 || numCols <= 0)
        throw invalid_argument("Scalar multiplcation requires a non-empty matrix");

    T *resultMatrix = new T[size()];
    size_t memSize = size() * sizeof(*mat);

    computeMethod.doMatrixScalarMultiplication(mat, resultMatrix, scalar, numCols, size());

    memcpy(mat, resultMatrix, memSize);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*(Matrix<T> &matrix)
{
    if (numCols != matrix.rows())
        throw invalid_argument("The number of columns in one matrix must equal the number of rows in the other matrix");

    // Since we use '+=' while doing the actual calculation, all values must be intialized prior to calculating the product
    T *resultMatrix = new int[size()];
    T *inputMatrix = matrix.flatten();

    for (int i = 0; i < size(); i++)
        resultMatrix[i] = 0;

    computeMethod.doMatrixMultiplication(mat, inputMatrix, resultMatrix, numRows, numCols, matrix.cols());

    mat = resultMatrix;
    
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator/(Matrix<T> &matrix)
{
    if (numCols != matrix.rows())
        throw invalid_argument("The number of columns in one matrix must equal the number of rows in the other matrix");

    return this * matrix.inverse();
}

template <typename T>
Matrix<T> &Matrix<T>::operator/(T scalar)
{
    if (numRows <= 0 || numCols <= 0)
        throw invalid_argument("Scalar multiplcation requires a non-empty matrix");

    T *resultMatrix = new T[size()];
    size_t memSize = size() * sizeof(*mat);

    computeMethod.doMatrixScalarDivision(mat, resultMatrix, scalar, numCols, size());

    memcpy(mat, resultMatrix, memSize);

    return *this;
}

template <typename T>
bool Matrix<T>::equals(Matrix<T> &matrix)
{
    T *flattenedComparisonMatrix = matrix.flatten();

    for (int i = 0; i < size(); i++)
        if (mat[i] != flattenedComparisonMatrix[i])
            return false;

    return true;
}