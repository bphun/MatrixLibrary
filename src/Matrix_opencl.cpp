#include "Matrix.h"

/**
 * Initialize an empty matrix with 0 rows and 0 columns
 */
template <typename T>
Matrix<T>::Matrix()
{
    initOpenCl();

    this->numRows = 0;
    this->numCols = 0;

    mat = new int[0];
}

/**
 * Initialize a matrix filled with zeros with the specified
 * number of rows and columns
 * 
 * @param rows Number of rows that the matrix will have
 * @param cols Number of columns that the matrix will have
 */
template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols)
{
    this->numRows = rows;
    this->numCols = cols;

    mat = new int[size()];

    for (int i = 0; i < size(); i++)
    {
        mat[i] = 0;
    }

    /*
     * memset doesn't appear to intialize all values of the array to 0 as it should with this
     * particular call. very strange 🤔. 
     */
    // memset(mat, 0, sizeof(int*));
}

/**
 * Initialize a matrix with values that are in an array of a numerical generic T.
 * This matrix will have the same number of rows and columns that
 * are specified by the user in the rows and cols parameters.
 * 
 * @param arr A pointer array of numerical generic T which will be loaded into the matrix
 * @param rows Number of rows that the matrix will have
 * @param cols Number of columns that the matrix will have
 */
template <typename T>
Matrix<T>::Matrix(T *arr, size_t rows, size_t cols)
{
    this->numRows = rows;
    this->numCols = cols;

    mat = new int[size()];

    for (int i = 0; i < size(); i++)
    {
        setElementAt(i / cols, i % cols, arr[i]);
    }
}

/**
 * Initialize a matrix with values that are in a vector of a numerical generic T.
 * This matrix will have the same number of rows and columns that
 * are specified by the user in the rows and cols parameters.
 * 
 * @param arr A vector with numerical generic T which will be loaded into the matrix
 * @param rows Number of rows that the matrix will have
 * @param cols Number of columns that the matrix will have
 */
template <typename T>
Matrix<T>::Matrix(vector<T> arr, size_t rows, size_t cols)
{
    this->numRows = rows;
    this->numCols = cols;

    mat = new int[size()];

    for (int i = 0; i < size(); i++)
    {
        setElementAt(i / cols, i % cols, arr[i]);
    }
}

/**
 * Initialize a matrix with the same contents as the specified matrix.
 * The new matrix will also have the same number of rows and colums as the speciifed matrix.
 * 
 * @param matrix The matrix that will be copied to the new matrix
 */
template <typename T>
Matrix<T>::Matrix(Matrix<T> &matrix)
{
    this->numRows = matrix.rows();
    this->numCols = matrix.cols();

    mat = new int[matrix.size()];

    memcpy(mat, matrix.flatten(), sizeof(int *));
}

template <typename T>
Matrix<T>::~Matrix()
{
    delete[] mat;
}

/**
 * @return Returns the number of colums that are in the matrix
 */
template <typename T>
size_t Matrix<T>::cols() const
{
    return this->numCols;
}

/**
 * @return Returns the number of rows that are in the matrix
 */
template <typename T>
size_t Matrix<T>::rows() const
{
    return this->numRows;
}

/**
 * @return Returns the total number of elements in the matrix by multiplying
 * the number of rows by the number of columns.
 */
template <typename T>
int Matrix<T>::size() const
{
    return numRows * numCols;
}

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
template <typename T>
T Matrix<T>::elementAt(int row, int col) const
{
    return mat[(row * numCols) + col];
}

/**
 * Sets the element at the specified row and column to the desired value.
 * 
 * @param row Row where the specified value will be placed
 * @param col Column where the specified value will be placed
 * @param value The value with type T, a generic number, that will be placed at the specified row and column
 */
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

/**
 * Prints a representation of the matrix to stdout
 */
template <typename T>
void Matrix<T>::print()
{
    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < numCols; c++)
        {
            cout << elementAt(r, c) << " ";
        }
        cout << endl;
    }
}

/**
 * Adds the contents of two matrices
 * 
 * @param matrix The matrix that will be combined with the contents of the calling matrix
 * 
 * @return A matrix that contains the values of the combined matrices 
 */
template <typename T>
Matrix<T> &Matrix<T>::operator+(Matrix<T> &matrix)
{
    if (numRows <= 0 || matrix.rows() <= 0 || numCols <= 0 || matrix.cols() <= 0)
        throw invalid_argument("Matrix addition requires non-empty matrices");

    if (numRows != matrix.rows() || numCols != matrix.cols())
        throw invalid_argument("Matrix addition requires two matrices of equal shape");

    T *resultArr = new int[size()];
    T *flattenedMatrix = matrix.flatten();

    for (int i = 0; i < size(); i++)
    {
        resultArr[i] = mat[i] + flattenedMatrix[i];
    }

    return *new Matrix<T>(resultArr, numRows, numCols);
}

/**
 * Subtracts the contents of two matrices
 * 
 * @param matrix The matrix that will be subtracted from with the contents of the calling matrix
 * 
 * @return A matrix that contains the values of the subtracted matrices 
 */
template <typename T>
Matrix<T> &Matrix<T>::operator-(Matrix<T> &matrix)
{
    if (numRows <= 0 || matrix.rows() <= 0 || numCols <= 0 || matrix.cols() <= 0)
        throw invalid_argument("Matrix subtraction requires non-empty matrices");

    if (numRows != matrix.rows() || numCols != matrix.cols())
        throw invalid_argument("Matrix subtraction requires two matrices of equal shape");

    T *resultArr = new int[size()];
    T *flattenedMatrix = matrix.flatten();

    for (int i = 0; i < size(); i++)
    {
        resultArr[i] = mat[i] - flattenedMatrix[i];
    }

    return *new Matrix<T>(resultArr, numRows, numCols);
}

/**
 * Multiplies the contents a matrix by a scalar constant
 * 
 * @param scalar The scalar that will be added to each element in the calling matrix
 * 
 * @return A matrix where element has increased by the value specified in scalar
 */
template <typename T>
Matrix<T> &Matrix<T>::operator*(T scalar)
{
    if (numRows <= 0 || numCols <= 0)
        throw invalid_argument("Scalar multiplcation requires a non-empty matrix");

    T *resultArr = new int[size()];

    for (int i = 0; i < size(); i++)
    {
        resultArr[i] = mat[i] * scalar;
    }

    return *new Matrix<T>(resultArr, numRows, numCols);
}

/**
 * Multiplies two matrices with each other
 * 
 * @param matrix that will be muliplied with the calling matrix
 * 
 * @return A matrix which contains the result of the multiplication of two matrices
 */
template <typename T>
Matrix<T> &Matrix<T>::operator*(Matrix<T> &matrix)
{
    if (numCols != matrix.rows())
        throw invalid_argument("The number of columns in one matrix must equal the number of rows in the other matrix");

    Matrix<T> *resultMatrix = new Matrix(numRows, matrix.cols());

    for (int r = 0; r < numRows; r++)
    {
        for (int c = 0; c < matrix.cols(); c++)
        {
            for (int k = 0; k < numCols; k++)
            {
                resultMatrix->setElementAt(r, c, resultMatrix->elementAt(r, c) + elementAt(r, k) * matrix.elementAt(k, c));
            }
        }
    }

    return *resultMatrix;
}

template <typename T>
Matrix<T> &Matrix<T>::operator/(Matrix<T> &matrix)
{
    if (numCols != matrix.rows())
        throw invalid_argument("The number of columns in one matrix must equal the number of rows in the other matrix");

    return this * matrix.inverse();
}

/**
 * Divides the contents of a matrix by a scalar value
 * 
 * @param The scalar quantity by which all the contents in the calling matrix will be dividied by
 * 
 * @return A matrix which contains the values of the calling matrix divided by the scalar quantity
 */
template <typename T>
Matrix<T> &Matrix<T>::operator/(T scalar)
{
    if (numRows <= 0 || numCols <= 0)
        throw invalid_argument("Scalar multiplcation requires a non-empty matrix");

    T *resultArr = new int[size()];

    for (int i = 0; i < size(); i++)
    {
        resultArr[i] = mat[i] / scalar;
    }

    return *new Matrix<T>(resultArr, numRows, numCols);
}

/**
 * Determines if two matrices contain the same elements
 * 
 * @return Returns true if the contents of the two matrices are equal and false if otherwise
 */
template <typename T>
bool Matrix<T>::equals(Matrix<T> &matrix)
{
    T *flattenedComparisonMatrix = matrix.flatten();

    for (int i = 0; i < size(); i++)
    {
        if (mat[i] != flattenedComparisonMatrix[i])
            return false;
    }

    return true;
}

/* MARK: OpenCL helper functions */

/**
 *  Initialize the OpenCL compute context and builds the compute program executable
 */
template <typename T>
void Matrix<T>::initOpenCl()
{
    createOpenClComputeContext();
    buildOpenClProgramExecutable(kernelFilePath);
}

/**
 * Creates an OpenCL compute context for future execution of kernels
 */
template <typename T>
void Matrix<T>::createOpenClComputeContext()
{
    deviceCount = 0;
    clGetPlatformIDs(0, 0, &deviceCount);

    clGetPlatformIDs(deviceCount, platformsIds, nullptr);

    err = clGetDeviceIDs(platformsIds[0], CL_DEVICE_TYPE_GPU, 1, &deviceId, nullptr);

    if (err != CL_SUCCESS)
    {
        printf("Failed to create OpenCL device group\n");
        exit(EXIT_FAILURE);
    }

    context = clCreateContext(0, 1, &deviceId, nullptr, nullptr, &err);
    if (!context)
    {
        printf("Failed to create an OpenCL compute context\n");
        exit(EXIT_FAILURE);
    }

    commands = clCreateCommandQueue(context, deviceId, 0, &err);
    if (!commands)
    {
        printf("Failed to create an OpenCL command queue\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Builds an OpenCL program executable with the kernel file at the specified path
 * 
 * @param kernelFilePath File path of the OpenCL Kernel that will be used at execution time
 */
template <typename T>
void Matrix<T>::buildOpenClProgramExecutable(string kernelFilePath)
{
    char *kernelSource;

    // Open the file
    FILE *file = fopen(&kernelFilePath[0u], "r");
    if (!file)
    {
        printf("Error opening kernel file %s\n", &kernelFilePath[0u]);
        exit(1);
    }

    // Get its size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Read the kernel code as a string
    kernelSource = (char *)malloc((size + 1) * sizeof(char));
    fread(kernelSource, 1, size * sizeof(char), file);
    kernelSource[size] = '\0';
    fclose(file);

    program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, nullptr, &err);
    if (!program)
    {
        printf("Failed to create OpenCL compute program\n");
    }

    err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS)
    {
        size_t buildInfoLength;
        char buffer[2048];

        printf("Failed to build OpenCL program executable\n");
        clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &buildInfoLength);

        printf("Program build info:\n%s", buffer);
        exit(1);
    }
}

/**
 * Loads an OpenCL kernel from a pre-existing program instance
 */
template <typename T>
void Matrix<T>::loadOpenClKernel(string kernelName)
{
    if (!program)
    {
        printf("No OpenCL program context exists to create the kernel");
        exit(1);
    }

    kernel = clCreateKernel(program, &kernelName[0u], &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Failed to create OpenCL compute kernel\n");
        exit(1);
    }
}
