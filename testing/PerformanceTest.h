#ifndef PERFORMANCE_TEST
#define PERFORMANCE_TEST

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <fstream>

#include "../src/Matrix.h"
#include "PerformanceTest.h"

#ifdef ENABLE_OPENCL
#define COMPUTE_DEVICE_TYPE "GPU"
#elif ENABLE_OPENMP
#define COMPUTE_DEVICE_TYPE "CPU_THREADED"
#else
#define COMPUTE_DEVICE_TYPE "CPU_SINGLE_THREADED"
#endif

#define MATRIX_ROWS 128
#define MATRIX_COLS 128
#define NUM_TRIALS_PER_TEST 10

using namespace std::chrono;

struct PerformanceTest
{
    string testName;
    double *executionTimes = new double[NUM_TRIALS_PER_TEST]; // Execution time of the test in nanoseconds
    void (*testfunction)(void);

    PerformanceTest(string testName, void (*testFunction)(void))
    {
        this->testName = testName;
        this->testfunction = testFunction;
    }

    string meanExecutionTime()
    {
        double average = 0.0;
        for (int i = 0; i < NUM_TRIALS_PER_TEST; i++)
            average += executionTimes[i];
        return formatTime(average / NUM_TRIALS_PER_TEST);
    }

    string medianExecutionTime()
    {
        if (NUM_TRIALS_PER_TEST % 2 == 0)
            return formatTime((executionTimes[NUM_TRIALS_PER_TEST / 2] + executionTimes[(NUM_TRIALS_PER_TEST + 1) / 2]) / 2);
        else
            return formatTime(executionTimes[NUM_TRIALS_PER_TEST / 2]);
    }

    string minExecutionTime()
    {
        double minExecutionTime = executionTimes[0];
        for (int i = 0; i < NUM_TRIALS_PER_TEST; i++)
            if (executionTimes[i] < minExecutionTime)
                minExecutionTime = executionTimes[i];
        return formatTime(minExecutionTime);
    }

    string maxExecutionTime()
    {
        double maxExecutionTime = executionTimes[0];
        for (int i = 0; i < NUM_TRIALS_PER_TEST; i++)
            if (executionTimes[i] > maxExecutionTime)
                maxExecutionTime = executionTimes[i];
        return formatTime(maxExecutionTime);
    }

    string formatTime(double time)
    {
        // printf("%0.4fns\n", time);
        return to_string(time) + "ns";
        // if (time >= 100000 && time < 1000000)
        //     return to_string(time / 1000) + "us"; //  Convert to microseconds
        // else if (time >= 1000000)
        //     return to_string(time / 1000000) + "ms"; //  Convert to milliseconds
        // else
        //     return to_string(time) + "ns"; //  Dont do any conversions, return time in nanoseconds
    }
};

#endif
