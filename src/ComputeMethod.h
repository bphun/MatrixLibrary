#ifndef COMPUTE_METHOD_H
#define COMPUTE_METHOD_H

template <typename T>
struct ComputeMethod
{
    void initialize();

    void deinitialize();

    void doMatrixAddition(T *, T *, T *, int, int);

    void doMatrixSubtraction(T *, T *, T *, int, int);

    void doMatrixMultiplication(T *, T *, T *, int, int, int);
    void doMatrixScalarMultiplication(T *, T *, T, int, int);

    void doMatrixDivision();
    void doMatrixScalarDivision(T *, T *, T, int, int);
};

#ifdef ENABLE_OPENCL
#include "OpenCL/ComputeMethod.cpp"
#elif ENABLE_OPENMP
#include "OpenMP/ComputeMethod.cpp"
#else
#include "SingleThreaded/ComputeMethod.cpp"
#endif

#endif /* COMPUTE_METHOD_H */