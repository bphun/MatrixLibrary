#include "../ComputeMethod.h"
#include "GpuCompute.h"

template <typename T>
void ComputeMethod<T>::initialize()
{
    initOpenCl();
}

template <typename T>
void ComputeMethod<T>::deinitialize()
{
    deinitOpenCl();
}

template <typename T>
void ComputeMethod<T>::doMatrixAddition(T *mat, T *inputMatrix, T *resultMatrix, int matNumCols, int matrixSize)
{
    size_t memSize = matrixSize * sizeof(*mat);

    loadOpenClKernel("matrixAddition");

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, inputMatrix, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultMatrix, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&matNumCols));

    executeKernel(memSize, deviceOutputArray, resultMatrix);

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);
}

template <typename T>
void ComputeMethod<T>::doMatrixSubtraction(T *mat, T *inputMatrix, T *resultMatrix, int numCols, int matrixSize)
{
    size_t memSize = matrixSize * sizeof(*mat);

    loadOpenClKernel("matrixSubtraction");

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, inputMatrix, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultMatrix, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel(memSize, deviceOutputArray, resultMatrix);

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);
}

template <typename T>
void ComputeMethod<T>::doMatrixMultiplication(T *mat, T *inputMatrix, T *resultMatrix, int matNumRows, int matNumCols, int inputMatrixCols)
{
    size_t memSize = (matNumRows * matNumCols) * sizeof(*mat);

    loadOpenClKernel("matrixMultiplication");

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceInputArrayB = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, inputMatrix, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultMatrix, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceInputArrayB));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&matNumCols));
    CL_CHECK(clSetKernelArg(kernel, 4, sizeof(int), (void *)&inputMatrixCols));

    executeKernel(memSize, deviceOutputArray, resultMatrix);

    releaseClMemObjects(deviceInputArrayA, deviceInputArrayB, deviceOutputArray);
}

template <typename T>
void ComputeMethod<T>::doMatrixScalarMultiplication(T *mat, T *resultMatrix, T scalar, int numCols, int matrixSize)
{
    size_t memSize = matrixSize * sizeof(*mat);

    loadOpenClKernel("scalarMultiplication");
    
    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, memSize, resultMatrix, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(int), (void *)&scalar));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel(memSize, deviceOutputArray, resultMatrix);

    releaseClMemObjects(deviceInputArrayA, deviceOutputArray);
}

template <typename T>
void ComputeMethod<T>::doMatrixDivision()
{
}

template <typename T>
void ComputeMethod<T>::doMatrixScalarDivision(T *mat, T *resultMatrix, T scalar, int numCols, int matrixSize)
{
    size_t memSize = matrixSize * sizeof(*mat);

    loadOpenClKernel("scalarDivision");

    cl_mem deviceInputArrayA = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, memSize, mat, &err));
    cl_mem deviceOutputArray = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_WRITE_ONLY, memSize, nullptr, &err));

    CL_CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&deviceInputArrayA));
    CL_CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&deviceOutputArray));
    CL_CHECK(clSetKernelArg(kernel, 2, sizeof(T), (void *)&scalar));
    CL_CHECK(clSetKernelArg(kernel, 3, sizeof(int), (void *)&numCols));

    executeKernel(memSize, deviceOutputArray, resultMatrix);

    releaseClMemObjects(deviceInputArrayA, deviceOutputArray);
}