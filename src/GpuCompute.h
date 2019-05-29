#ifndef GPU_COMPUTE_H
#define GPU_COMPUTE_H

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

int err;
bool kernelCreated = false;

cl_device_id deviceId;     //   compute device id
cl_context context;        //   compute context
cl_command_queue commands; //   compute command queue
cl_program program;        //   compute program
cl_kernel kernel;          //   compute kernel

cl_uint deviceCount;
cl_uint platformCount;

cl_platform_id platforms[100];
cl_device_id devices[100];

cl_mem deviceInputArrayA;
cl_mem deviceInputArrayB;
cl_mem deviceOutputArray;

size_t localWorkSize[2], globalWorkSize[2];

void pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
    printf("OpenCL Error (via pfn_notify): %s\n", errinfo);
}

void detectDevices()
{
    CL_CHECK(clGetPlatformIDs(100, platforms, &platformCount));
    if (platformCount == 0)
        exit(1);

    CL_CHECK(clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 100, devices, &deviceCount));

    // printf("%d OpenCL platform(s) found:\n", platformCount);
    // for (int i = 0; i < platformCount; i++)
    // {
    //     char buffer[10240];
    //     printf("  -- %d --\n", i);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 10240, buffer, NULL));
    //     printf("\tPROFILE: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 10240, buffer, NULL));
    //     printf("\tVERSION: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 10240, buffer, NULL));
    //     printf("\tNAME: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 10240, buffer, NULL));
    //     printf("\tVENDOR: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL));
    //     printf("\tEXTENSIONS: %s\n", buffer);
    // }

    // printf("%d OpenCL device(s) found on platform:\n", platformCount + 1);
    // for (int i = 0; i < deviceCount; i++)
    // {
    //     char buffer[10240];
    //     cl_uint buf_uint;
    //     cl_ulong buf_ulong;
    //     printf("  -- %d --\n", i);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL));
    //     printf("\tDEVICE_NAME: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL));
    //     printf("\tDEVICE_VENDOR: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL));
    //     printf("\tDEVICE_VERSION: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL));
    //     printf("\tDRIVER_VERSION: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL));
    //     printf("\tDEVICE_MAX_COMPUTE_UNITS: %u\n", (unsigned int)buf_uint);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL));
    //     printf("\tDEVICE_MAX_CLOCK_FREQUENCY: %u\n", (unsigned int)buf_uint);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
    //     printf("\tDEVICE_GLOBAL_MEM_SIZE: %llu\n", (unsigned long long)buf_ulong);
    // }
}

/**
 * Creates an OpenCL compute context for future execution of kernels
 */
void createOpenClComputeContext()
{
    context = CL_CHECK_ERR(clCreateContext(nullptr, 1, devices, pfn_notify, nullptr, &err));
    commands = CL_CHECK_ERR(clCreateCommandQueue(context, devices[0], 0, &err));
}

/**
 * Builds an OpenCL program executable with the kernel file at the specified path
 * 
 * @param KERNEL_FILE_PATH File path of the OpenCL Kernel that will be used at execution time
 */
void buildOpenClProgramExecutable(string kernelFilePath)
{
    char *kernelSource;

    // Open the file
    FILE *file = fopen(&kernelFilePath[0u], "r");
    if (!file)
    {
        printf("Error opening kernel file %s\n", &kernelFilePath[0u]);
        exit(1);
    }

    // Get kernel file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Read the kernel code as a string
    kernelSource = (char *)malloc((fileSize + 1) * sizeof(char));
    fread(kernelSource, 1, fileSize * sizeof(char), file);
    kernelSource[fileSize] = '\0';
    fclose(file);

    program = CL_CHECK_ERR(clCreateProgramWithSource(context, 1, (const char **)&kernelSource, nullptr, &err));

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
 * Creates an OpenCL kernel from a pre-existing program instance
 * 
 * @param kernelName Name of the kernel that will be created
 */
void loadOpenClKernel(string kernelName)
{
    if (!program)
    {
        printf("No OpenCL program context exists to create the kernel\n");
        exit(1);
    }

    kernel = CL_CHECK_ERR(clCreateKernel(program, &kernelName[0u], &err));

    kernelCreated = true;
}

template <class... params>
void releaseClMemObjects(params... memObjects)
{
    for (auto &&memObject : {memObjects...})
        CL_CHECK(clReleaseMemObject(memObject));
}

template <typename T>
void executeKernel(cl_command_queue commandQueue, cl_kernel kernel, int outputArraySize, cl_mem deviceOutputArray, T *hostOutputArray)
{
    cl_event kernelCompletion;

    CL_CHECK(clEnqueueNDRangeKernel(commandQueue, kernel, 2, nullptr, globalWorkSize, localWorkSize, 0, nullptr, &kernelCompletion));

    CL_CHECK(clFinish(commandQueue));

    CL_CHECK(clWaitForEvents(1, &kernelCompletion));
    CL_CHECK(clReleaseEvent(kernelCompletion));

    CL_CHECK(clEnqueueReadBuffer(commandQueue, deviceOutputArray, CL_TRUE, 0, outputArraySize, hostOutputArray, 0, nullptr, nullptr));

    CL_CHECK(clFinish(commandQueue));
}

/**
 * Initialize the OpenCL compute context and builds the compute program executable
 * 
 * TODO: run on other thread to improve intialization times 
 */
void initOpenCl()
{
    detectDevices();
    createOpenClComputeContext();
    buildOpenClProgramExecutable(KERNEL_FILE_PATH);

    localWorkSize[0] = 16;
    localWorkSize[1] = 16;
    globalWorkSize[0] = 1024;
    globalWorkSize[1] = 1024;
}

void deinitOpenCl()
{
    CL_CHECK(clReleaseProgram(program));
    // if (kernelCreated)
    // CL_CHECK(clReleaseKernel(kernel));
    CL_CHECK(clReleaseCommandQueue(commands));
    CL_CHECK(clReleaseContext(context));
}

#endif