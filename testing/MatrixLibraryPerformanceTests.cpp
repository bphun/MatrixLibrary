#include "PerformanceTest.h"

void collectExecutionTime(double &, void (*)(void));
void printProgress(double progress);

void emptyConstructorTest();
void specifiedRowColConstructorTest();
void specifiedMatrixArrayConstructorTest();
void scalarDivisionTest();
void matrixMultiplicationTest();
void scalarMultiplicationTest();
void matrixSubtractionTest();
void matrixAdditionTest();

int* arrA = new int [MATRIX_ROWS * MATRIX_COLS];
int* arrB = new int [MATRIX_ROWS * MATRIX_COLS];

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
    performanceTests[3] = new PerformanceTest("Matrix addition", matrixAdditionTest);
    performanceTests[4] = new PerformanceTest("Scalar division", scalarDivisionTest);
    performanceTests[5] = new PerformanceTest("Scalar multiplication", scalarMultiplicationTest);
    performanceTests[6] = new PerformanceTest("Matrix multiplication", matrixMultiplicationTest);
    performanceTests[7] = new PerformanceTest("Matrix subtraction", matrixSubtractionTest);

    printf("Executing %d test(s) with %dx%d matrices using %s. %d iterations/test\n",
           numTests, 
           MATRIX_ROWS,
           MATRIX_COLS,
           COMPUTE_DEVICE_TYPE,
           NUM_TRIALS_PER_TEST);

    for (int i = 0; i < numTests; i++) {
        PerformanceTest *performanceTest = performanceTests[i];

        fstream testResultsFile("../testing/testResults/" + performanceTest->testName + "_" + COMPUTE_DEVICE_TYPE + ".csv", 
                                ios::out | ios::binary);

        testResultsFile << performanceTest->testName << "\n";

        printf("Running Test: %s\n", performanceTest->testName.c_str());
        for (int n = 0; n < NUM_TRIALS_PER_TEST; n++) {
            printProgress((double)n / (double)NUM_TRIALS_PER_TEST);
            collectExecutionTime(performanceTest->executionTimes[n], performanceTest->testfunction);
            testResultsFile << float(performanceTest->executionTimes[n]) << std::fixed << "\n";
        }

        printf("Test done: %s test completed. (Min: %s, Max: %s, Mean: %s, Median: %s)\n",
               performanceTest->testName.c_str(),
               performanceTest->minExecutionTime().c_str(),
               performanceTest->maxExecutionTime().c_str(),
               performanceTest->meanExecutionTime().c_str(),
               performanceTest->medianExecutionTime().c_str());

        testResultsFile.close();
    }

    delete[] arrA;
    delete[] arrB;

    for (int i = 0 ; i < numTests; i++)
        if (performanceTests[i] != nullptr)
            delete performanceTests[i];

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

    matrixA * 2;
}

void printProgress(double progress)
{
    std::cout << "[";
    int pos = 70 * progress;
    for (int i = 0; i < 70; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}