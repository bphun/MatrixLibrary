#ifndef PERFORMANCE_TEST
#define PERFORMANCE_TEST

#define MATRIX_ROWS 512
#define MATRIX_COLS 512
#define NUM_TRIALS_PER_TEST 50

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
        {
            if (minExecutionTime < executionTimes[i])
                minExecutionTime = executionTimes[i];
        }
        return formatTime(minExecutionTime);
    }

    string maxExecutionTime()
    {
        double maxExecutionTime = executionTimes[0];
        for (int i = 0; i < NUM_TRIALS_PER_TEST; i++)
        {
            if (maxExecutionTime > executionTimes[i])
                maxExecutionTime = executionTimes[i];
        }
        return formatTime(maxExecutionTime);
    }

    string formatTime(double time)
    {
        // printf("%0.4f\n", executionTimeNs);
        if (time >= 100000 && time < 1000000)
            return to_string(time / 1000) + "us"; //  Convert to microseconds
        else if (time >= 1000000)
            return to_string(time / 1000000) + "ms"; //  Convert to milliseconds
        else
            return to_string(time) + "ns"; //  Dont do any conversions, return time in nanoseconds
    }

    void writeDataToCsv(fstream file)
    {
        
    }
};

#endif