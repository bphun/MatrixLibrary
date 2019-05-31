#include "GpuCompute.h"

void pfn_notify_create_context(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
    printf("OpenCL Error (via pfn_notify): %s\n", errinfo);
}

void pfn_notify_build_program(cl_program program, void *user_data)
{
}

void detectDevices()
{
    CL_CHECK(clGetPlatformIDs(10, platforms, &platformCount));
    if (platformCount == 0)
        exit(1);

    CL_CHECK(clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 10, devices, &deviceCount));

    // _lock.lock();
    // clDevicesDetected = true;
    // _cond.notify_all();
    // _lock.unlock();
#ifdef DEBUG
    // printf("%d OpenCL platform(s) found:\n", platformCount);
    // for (int i = 0; i < platformCount; i++)
    // {
    //     char buffer[10240];
    //     printf("  -- %d --\n", i);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 10240, buffer, nullptr));
    //     printf("\tPROFILE: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 10240, buffer, nullptr));
    //     printf("\tVERSION: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 10240, buffer, nullptr));
    //     printf("\tNAME: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 10240, buffer, nullptr));
    //     printf("\tVENDOR: %s\n", buffer);
    //     CL_CHECK(clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 10240, buffer, nullptr));
    //     printf("\tEXTENSIONS: %s\n", buffer);
    // }

    // printf("%d OpenCL device(s) found on platform:\n", platformCount + 1);
    // for (int i = 0; i < deviceCount; i++)
    // {
    //     char buffer[10240];
    //     cl_uint buf_uint;
    //     cl_ulong buf_ulong;
    //     printf("  -- %d --\n", i);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, nullptr));
    //     printf("\tDEVICE_NAME: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, nullptr));
    //     printf("\tDEVICE_VENDOR: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, nullptr));
    //     printf("\tDEVICE_VERSION: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, nullptr));
    //     printf("\tDRIVER_VERSION: %s\n", buffer);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, nullptr));
    //     printf("\tDEVICE_MAX_COMPUTE_UNITS: %u\n", (unsigned int)buf_uint);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, nullptr));
    //     printf("\tDEVICE_MAX_CLOCK_FREQUENCY: %u\n", (unsigned int)buf_uint);
    //     CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, nullptr));
    //     printf("\tDEVICE_GLOBAL_MEM_SIZE: %llu\n", (unsigned long long)buf_ulong);
    // }
#endif /* DEBUG */
}

void createOpenClComputeContext()
{
    // _lock.lock();
    // if (clDevicesDetected == false)
    // {
    //     _cond.wait(_lock);
    // }
    // _lock.unlock();

    context = CL_CHECK_ERR(clCreateContext(nullptr, 1, devices, 0, nullptr, &err));
    // context = CL_CHECK_ERR(clCreateContext(nullptr, 1, devices, pfn_notify_create_context, nullptr, &err));

    commands = CL_CHECK_ERR(clCreateCommandQueue(context, devices[0], 0, &err));

    // _lock.lock();
    // clContextCreated = true;
    // _cond.notify_all();
    // _lock.unlock();
    // for (int i = 0; i < deviceCount; i++)
    //     commands = CL_CHECK_ERR(clCreateCommandQueue(context, devices[i], 0, &err));
}

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

    // _lock.lock();
    // if (clContextCreated == false)
    // {
    //     _cond.wait(_lock);
    // }
    // _lock.unlock();

    program = CL_CHECK_ERR(clCreateProgramWithSource(context, 1, (const char **)&kernelSource, nullptr, &err));

    err = clBuildProgram(program, 0, nullptr, nullptr, pfn_notify_build_program, nullptr);
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

void loadOpenClKernel(string kernelName)
{
    if (!program)
    {
        printf("No OpenCL program context exists to create the kernel\n");
        exit(1);
    }

    kernel = CL_CHECK_ERR(clCreateKernel(program, &kernelName[0u], &err));

    kernelCreated = true;

#if DEBUG
    printf("Loaded Kernel:\n");
    char buffer[10240];
    cl_uint numKernelArgs;
    clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, 10240, buffer, nullptr);
    printf("\tKernel name: %s\n", buffer);
    clGetKernelInfo(kernel, CL_KERNEL_NUM_ARGS, sizeof(cl_uint), &numKernelArgs, nullptr);
    printf("\tKernel Arguments: %d\n", numKernelArgs);
#endif
}

void wait(cl_command_queue commandQueue, cl_event kernelCompletion)
{
    CL_CHECK(clFinish(commandQueue));

    CL_CHECK(clWaitForEvents(1, &kernelCompletion));
    CL_CHECK(clReleaseEvent(kernelCompletion));

    kernelCompletion = nullptr;
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

    wait(commandQueue, kernelCompletion);

    CL_CHECK(clEnqueueReadBuffer(commandQueue, deviceOutputArray, CL_TRUE, 0, outputArraySize, hostOutputArray, 0, nullptr, &kernelCompletion));

    wait(commandQueue, kernelCompletion);
}

void initOpenCl()
{
    // thread createClComputeContextThread(createOpenClComputeContext);

    detectDevices();

    createOpenClComputeContext();

    buildOpenClProgramExecutable(KERNEL_FILE_PATH);

    // createClComputeContextThread.join();

    localWorkSize[0] = 16;
    localWorkSize[1] = 16;
    globalWorkSize[0] = 512;
    globalWorkSize[1] = 512;
}

void deinitOpenCl()
{
    CL_CHECK(clReleaseProgram(program));
    // if (kernelCreated)
    //     CL_CHECK(clReleaseKernel(kernel));
    // CL_CHECK(clReleaseCommandQueue(commands));
    // CL_CHECK(clReleaseContext(context));
}
