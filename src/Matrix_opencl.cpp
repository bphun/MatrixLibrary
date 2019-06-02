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
    size_t memSize = size() * sizeof(*mat);

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, flattenedArr, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultArr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel();

    getKernelOutputArray(memSize, deviceOutputArray, resultArr);

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);

    memcpy(mat, resultArr, memSize);

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
    size_t memSize = size() * sizeof(*mat);

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, flattenedArr, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultArr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel();

    getKernelOutputArray(memSize, deviceOutputArray, resultArr);

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);

    memcpy(mat, resultArr, memSize);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*(T scalar)
{
    if (numRows <= 0 || numCols <= 0)
        throw invalid_argument("Scalar division requires a non-empty matrix");

    loadOpenClKernel("scalarMultiplication");

    T *resultArr = new T[size()];
    size_t memSize = size() * sizeof(*mat);

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultArr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(int), (void *)&scalar));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel();

    getKernelOutputArray(memSize, deviceOutputArray, resultArr);

    releaseClMemObjects(deviceInputArrayA, deviceOutputArray);

    memcpy(mat, resultArr, memSize);

    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*(Matrix<T> &matrix)
{
    if (numCols != matrix.rows())
        throw invalid_argument("The number of columns in one matrix must equal the number of rows in the other matrix");

    loadOpenClKernel("matrixMultiplication");

    T *resultArr = new T[size()];
    T *flattenedArr = matrix.flatten();
    size_t memSize = size() * sizeof(*mat);
    int matrixCols = matrix.cols();

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, flattenedArr, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultArr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));
    CL_CHECK(clSetKernelArg(kernel, 4, sizeof(int), (void *)&matrixCols));

    executeKernel();

    getKernelOutputArray(memSize, deviceOutputArray, resultArr);

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);

    memcpy(mat, resultArr, memSize);

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
        throw invalid_argument("Scalar division requires a non-empty matrix");

    T *resultArr = new T[size()];
    size_t memSize = size() * sizeof(*mat);

    loadOpenClKernel("scalarDivision");

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_WRITE_ONLY, memSize, nullptr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(T), (void *)&scalar));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel();

    getKernelOutputArray(memSize, deviceOutputArray, resultArr);

    releaseClMemObjects(deviceInputArrayA, deviceOutputArray);

    memcpy(mat, resultArr, memSize);

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