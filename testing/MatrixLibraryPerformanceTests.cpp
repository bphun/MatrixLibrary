#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <fstream>

#include "../src/Matrix.h"
#include "PerformanceTest.h"

using namespace std::chrono;

//  Seg fault may occur when the matrix has more than 1,440,000 elements

void collectExecutionTime(double &, void (*)(void));

void emptyConstructorTest();
void specifiedRowColConstructorTest();
void specifiedMatrixArrayConstructorTest();
void scalarDivisionTest();
void matrixMultiplicationTest();
void scalarMultiplicationTest();
void matrixSubtractionTest();
void matrixAdditionTest();

int arrA[MATRIX_ROWS * MATRIX_COLS];
int arrB[MATRIX_ROWS * MATRIX_COLS];

int main()
{
    const int numTests = 8;
    PerformanceTest *performanceTests[numTests];

    for (int i = 0; i < MATRIX_ROWS * MATRIX_COLS; i++)
    {
        arrA[i] = i;
        arrB[i] = (MATRIX_ROWS * MATRIX_COLS) - i;
    }

    performanceTests[0] = new PerformanceTest("Empty Constructor", emptyConstructorTest);
    performanceTests[1] = new PerformanceTest("Specified row/col Constructor", specifiedRowColConstructorTest);
    performanceTests[2] = new PerformanceTest("Specified matrix array", specifiedMatrixArrayConstructorTest);
    performanceTests[3] = new PerformanceTest("Scalar division", scalarDivisionTest);
    performanceTests[4] = new PerformanceTest("Matrix multiplication", matrixMultiplicationTest);
    performanceTests[5] = new PerformanceTest("Scalar multiplication", scalarMultiplicationTest);
    performanceTests[6] = new PerformanceTest("Matrix subtraction", matrixSubtractionTest);
    performanceTests[7] = new PerformanceTest("Matrix addition", matrixAdditionTest);

    printf("Executing %d test(s) with %dx%d matrices...\n", numTests, MATRIX_ROWS, MATRIX_COLS);

    for (int i = 0; i < numTests; i++)
    {
        for (int n = 0; n < NUM_TRIALS_PER_TEST; n++)
            collectExecutionTime(performanceTests[i]->executionTimes[n], performanceTests[i]->testfunction);

        printf("Test: %s test completed. (Min: %s, Max: %s, Mean: %s, Median: %s)\n",
               performanceTests[i]->testName.c_str(),
               performanceTests[i]->minExecutionTime().c_str(),
               performanceTests[i]->maxExecutionTime().c_str(),
               performanceTests[i]->meanExecutionTime().c_str(),
               performanceTests[i]->medianExecutionTime().c_str());
    }

    return 0;
}

void collectExecutionTime(double &executionTimeNs, void (*testFunction)(void))
{
    high_resolution_clock::time_point testStartTime = high_resolution_clock::now();

    testFunction();

    high_resolution_clock::time_point testEndTime = high_resolution_clock::now();

    executionTimeNs = duration_cast<nanoseconds>(testEndTime - testStartTime).count();
}

void emptyConstructorTest()
{
    Matrix<int> mat;
}

void specifiedRowColConstructorTest()
{
    Matrix<int> mat(MATRIX_ROWS, MATRIX_COLS);
}

void specifiedMatrixArrayConstructorTest()
{
    Matrix<int> matrix(arrA, MATRIX_ROWS, MATRIX_COLS);
}

void matrixAdditionTest()
{
    Matrix<int> matrixA(arrA, MATRIX_ROWS, MATRIX_COLS);
    Matrix<int> matrixB(arrB, MATRIX_ROWS, MATRIX_COLS);

    matrixA + matrixB;
}

void matrixSubtractionTest()
{
    Matrix<int> matrixA(arrA, MATRIX_ROWS, MATRIX_COLS);
    Matrix<int> matrixB(arrB, MATRIX_ROWS, MATRIX_COLS);

    matrixA - matrixB;
}

void scalarMultiplicationTest()
{
    Matrix<int> matrixA(arrA, MATRIX_ROWS, MATRIX_COLS);

    matrixA * 2;
}

void matrixMultiplicationTest()
{
    Matrix<int> matrixA(arrA, MATRIX_ROWS, MATRIX_COLS);
    Matrix<int> matrixB(arrB, MATRIX_ROWS, MATRIX_COLS);

    matrixA *matrixB;
}

void scalarDivisionTest()
{
    Matrix<int> matrixA(arrA, MATRIX_ROWS, MATRIX_COLS);

    matrixA / 2;
}