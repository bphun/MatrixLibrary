#ifndef GPU_COMPUTE_H
#define GPU_COMPUTE_H

#if __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
#include <string>
#include <thread>
#include <condition_variable>

using namespace std;

#define CL_CHECK(_expr)                                                         \
    do                                                                          \
    {                                                                           \
        cl_int _err = _expr;                                                    \
        if (_err == CL_SUCCESS)                                                 \
            break;                                                              \
        fprintf(stderr, "OpenCL Error: '%s' returned %d\n", #_expr, (int)_err); \
        abort();                                                                \
    } while (0)

#define CL_CHECK_ERR(_expr)                                                         \
    ({                                                                              \
        cl_int _err = CL_INVALID_VALUE;                                             \
        decltype(_expr) _ret = _expr;                                               \
        if (!_ret || err != CL_SUCCESS)                                             \
        {                                                                           \
            fprintf(stderr, "OpenCL Error: '%s' returned %d\n", #_expr, (int)_err); \
            abort();                                                                \
        }                                                                           \
        _ret;                                                                       \
    })

#define KERNEL_FILE_PATH "../src/gpuComputeKernels.cl"

mutex _lock;
condition_variable_any _cond;
bool clContextCreated;

int err;
bool kernelCreated = false;
bool clProgramCreated = false;
size_t localWorkSize[2], globalWorkSize[2];

cl_device_id deviceId;     //   compute device id
cl_context context;        //   compute context
cl_command_queue commands; //   compute command queue
cl_program program;        //   compute program
cl_kernel kernel;          //   compute kernel

cl_uint deviceCount;
cl_uint platformCount;

cl_platform_id platforms[10];
cl_device_id devices[10];

cl_mem deviceInputArrayA;
cl_mem deviceInputArrayB;
cl_mem deviceOutputArray;

void pfn_notify(const char *, const void *, size_t, void *);

void detectDevices();

/**
 * Creates an OpenCL compute context for future execution of kernels
 */
void createOpenClComputeContext();

/**
 * Builds an OpenCL program executable with the kernel file at the specified path
 * 
 * @param KERNEL_FILE_PATH File path of the OpenCL Kernel that will be used at execution time
 */
void buildOpenClProgramExecutable(string);

/**
 * Creates an OpenCL kernel from a pre-existing program instance
 * 
 * @param kernelName Name of the kernel that will be created
 */
void loadOpenClKernel(string);

/**
 * Releases OpenCL cl_mem references
 * 
 * @param memObjects The cl_mem references that will be released
 */
template <class... params>
void releaseClMemObjects(params...);

void wait(cl_command_queue, cl_event);

/**
 * Executes a specified kernel
 * 
 * @param commandQueue Command queue that will be executed by the kernel
 * @param kernel The kernel that will be executed
 */
template <typename T>
void executeKernel(cl_command_queue commandQueue, cl_kernel, int, cl_mem, T *);

/**
 * Initialize the OpenCL compute context and builds the compute program executable
 * 
 * TODO: run on other thread to improve intialization times 
 */
void initOpenCl();

/**
 * Releases OpenCL context, kernel, program, and command queue
 */
void deinitOpenCl();

#include "GpuCompute.cpp"
#endif /* GPU_COMPUTE_H */
