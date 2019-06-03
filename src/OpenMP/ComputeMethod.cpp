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
}

template <typename T>
void ComputeMethod<T>::doMatrixSubtraction(T *mat, T *inputMatrix, T *resultMatrix, int numCols, int matrixSize)
{
}

template <typename T>
void ComputeMethod<T>::doMatrixMultiplication(T *mat, T *inputMatrix, T *resultMatrix, int matNumRows, int matNumCols, int inputMatrixCols)
{
}

template <typename T>
void ComputeMethod<T>::doMatrixScalarMultiplication(T *mat, T *resultMatrix, T scalar, int numCols, int matrixSize)
{
}

template <typename T>
void ComputeMethod<T>::doMatrixDivision()
{
}

template <typename T>
void ComputeMethod<T>::doMatrixScalarDivision(T *mat, T *resultMatrix, T scalar, int numCols, int matrixSize)
{
}