#include "../ComputeMethod.h"

template <typename T>
void ComputeMethod<T>::initialize()
{
}

template <typename T>
void ComputeMethod<T>::deinitialize()
{
}

template <typename T>
void ComputeMethod<T>::doMatrixAddition(T *mat, T *inputMatrix, T *resultMatrix, int numCols, int matrixSize)
{
    for (int i = 0; i < matrixSize; i++)
        resultMatrix[i] = mat[i] + inputMatrix[i];
}

template <typename T>
void ComputeMethod<T>::doMatrixSubtraction(T *mat, T *inputMatrix, T *resultMatrix, int numCols, int matrixSize)
{
    for (int i = 0; i < matrixSize; i++)
        resultMatrix[i] = mat[i] - inputMatrix[i];
}

template <typename T>
void ComputeMethod<T>::doMatrixMultiplication(T *mat, T *inputMatrix, T *resultMatrix, int matNumRows, int matNumCols, int inputMatrixCols)
{
    for (int r = 0; r < matNumRows; r++) {
        for (int c = 0; c < inputMatrixCols; c++)
            for (int k = 0; k < matNumCols; k++) {
                resultMatrix[(r * matNumCols) + c] += mat[(r * matNumCols) + k] * inputMatrix[(k * matNumCols) + c];
            }
    }
}

template <typename T>
void ComputeMethod<T>::doMatrixScalarMultiplication(T *mat, T *resultMatrix, T scalar, int numCols, int matrixSize)
{
    for (int i = 0; i < matrixSize; i++)
        resultMatrix[i] = mat[i] * scalar;
}

template <typename T>
void ComputeMethod<T>::doMatrixDivision()
{
}

template <typename T>
void ComputeMethod<T>::doMatrixScalarDivision(T *mat, T *resultMatrix, T scalar, int numCols, int matrixSize)
{
    for (int i = 0; i < matrixSize; i++)
        resultMatrix[i] = mat[i] / scalar;
}