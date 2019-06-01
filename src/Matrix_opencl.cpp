#include "Matrix.h"

template <typename T>
Matrix<T>::Matrix()
{
    initOpenCl();

    this->numRows = 0;
    this->numCols = 0;

    mat = new T[0];
}

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols)
{
    initOpenCl();

    this->numRows = rows;
    this->numCols = cols;

    mat = new T[size()];

    for (int i = 0; i < size(); i++)
        mat[i] = 0;
}

template <typename T>
Matrix<T>::Matrix(T *arr, size_t rows, size_t cols)
{
    initOpenCl();

    this->numRows = rows;
    this->numCols = cols;

    mat = new T[size()];

    for (int i = 0; i < size(); i++)
        setElementAt(i / cols, i % cols, arr[i]);
}

template <typename T>
Matrix<T>::Matrix(vector<T> arr, size_t rows, size_t cols)
{
    initOpenCl();

    this->numRows = rows;
    this->numCols = cols;

    mat = new T[size()];

    for (int i = 0; i < size(); i++)
        setElementAt(i / cols, i % cols, arr[i]);
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> &matrix)
{
    initOpenCl();

    this->numRows = matrix.rows();
    this->numCols = matrix.cols();

    mat = new T[matrix.size()];

    memcpy(mat, matrix.flatten(), sizeof(int *));
}

template <typename T>
Matrix<T>::~Matrix()
{
    delete[] mat;
    deinitOpenCl();
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

    loadOpenClKernel("matrixAddition");

    T *resultArr = new T[size()];
    T *flattenedArr = matrix.flatten();
    int memSize = size() * sizeof(T *);

    deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, flattenedArr, &err));
    deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultArr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel();

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);

    mat = resultArr;

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-(Matrix<T> &matrix)
{
    if (numRows <= 0 || matrix.rows() <= 0 || numCols <= 0 || matrix.cols() <= 0)
        throw invalid_argument("Matrix subtraction requires non-empty matrices");

    if (numRows != matrix.rows() || numCols != matrix.cols())
        throw invalid_argument("Matrix subtraction requires two matrices of equal shape");

    loadOpenClKernel("matrixSubtraction");

    T *resultArr = new T[size()];
    T *flattenedArr = matrix.flatten();
    int memSize = size() * sizeof(T *);

    deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, flattenedArr, &err));
    deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultArr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel();

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);

    mat = resultArr;

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*(T scalar)
{
    if (numRows <= 0 || numCols <= 0)
        throw invalid_argument("Scalar multiplcation requires a non-empty matrix");

    T *resultArr = new T[size()];

    int memSize = size() * sizeof(T *);

    loadOpenClKernel("scalarMultiplication");

    deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_WRITE_ONLY, memSize, nullptr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(T), (void *)&scalar));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    // executeKernel();

    releaseClMemObjects(deviceInputArrayA, deviceOutputArray);

    mat = resultArr;

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*(Matrix<T> &matrix)
{
    if (numCols != matrix.rows())
        throw invalid_argument("The number of columns in one matrix must equal the number of rows in the other matrix");

    Matrix<T> *resultMatrix = new Matrix(numRows, matrix.cols());

    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < matrix.cols(); c++)
            for (int k = 0; k < numCols; k++)
                resultMatrix->setElementAt(r, c, resultMatrix->elementAt(r, c) + elementAt(r, k) * matrix.elementAt(k, c));

    return *resultMatrix;
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
        throw invalid_argument("Scalar division requires a non-empty matrix");

    T *resultArr = new T[size()];

    int memSize = size() * sizeof(T *);

    loadOpenClKernel("scalarMultiplication");

    deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_WRITE_ONLY, memSize, nullptr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(T), (void *)&scalar));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    // executeKernel();

    releaseClMemObjects(deviceInputArrayA, deviceOutputArray);

    mat = resultArr;

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