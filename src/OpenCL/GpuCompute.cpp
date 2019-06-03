#include "GpuCompute.h"

void pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
    printf("OpenCL Error (via pfn_notify): %s\n", errinfo);
}

void detectDevices()
{
    CL_CHECK(clGetPlatformIDs(10, platforms, &platformCount));

    if (platformCount == 0)
        exit(1);

    CL_CHECK(clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 10, devices, &deviceCount));

    _lock.lock();
    devicesDetected = true;
    _cond.notify_all();
    _lock.unlock();

#if DEBUG
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
#endif
}

void createOpenClComputeContext()
{
    _lock.lock();
    if (!devicesDetected)
        _cond.wait(_lock);
    _lock.unlock();

    context = CL_CHECK_ERR(clCreateContext(nullptr, deviceCount, devices, pfn_notify, nullptr, &err));

    _lock.lock();
    clContextCreated = true;
    _cond.notify_all();
    _lock.unlock();

    commands = CL_CHECK_ERR(clCreateCommandQueue(context, devices[0], 0, &err));
}

void buildOpenClProgramExecutable(string kernelFilePath)
{
    char *kernelSource;

    // Open the file
    FILE *file = fopen(&kernelFilePath[0u], "r");
    if (!file) {
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

    _lock.lock();
    if (!clContextCreated)
        _cond.wait(_lock);
    _lock.unlock();

    program = CL_CHECK_ERR(clCreateProgramWithSource(context, 1, (const char **)&kernelSource, nullptr, &err));

    err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        size_t buildInfoLength;
        char buffer[2048];

        printf("Failed to build OpenCL program executable\n");
        CL_CHECK(clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &buildInfoLength));

        printf("Program build info:\n%s", buffer);
        exit(1);
    }
}

void loadOpenClKernel(string kernelName)
{
    kernel = CL_CHECK_ERR(clCreateKernel(program, &kernelName[0u], &err));
}

template <class... params>
void releaseClMemObjects(params... memObjects)
{
    for (auto &&memObject : {memObjects...})
        CL_CHECK(clReleaseMemObject(memObject));
}

void wait(cl_event event)
{
    CL_CHECK(clFinish(commands));
    CL_CHECK(clFlush(commands));

    CL_CHECK(clWaitForEvents(1, &event));
    CL_CHECK(clReleaseEvent(event));
}

template <typename T>
void executeKernel(size_t memSize, cl_mem deviceOutputArray, T *resultArray)
{
    cl_event kernelExecEvent;

    CL_CHECK(clEnqueueNDRangeKernel(commands, kernel, 2, nullptr, globalWorkSize, localWorkSize, 0, nullptr, &kernelExecEvent));

    wait(kernelExecEvent);

    getKernelOutputArray(memSize, deviceOutputArray, resultArray);
}

template <typename T>
void getKernelOutputArray(size_t outputArraySize, cl_mem deviceOutputArray, T *hostOutputArray)
{
    cl_event bufferReadEvent;

    CL_CHECK(clEnqueueReadBuffer(commands, deviceOutputArray, CL_TRUE, 0, outputArraySize, hostOutputArray, 0, nullptr, &bufferReadEvent));

    wait(bufferReadEvent);
}

void initOpenCl()
{
    // thread createComputeContextThread(createOpenClComputeContext);

    detectDevices();
    createOpenClComputeContext();
    buildOpenClProgramExecutable(KERNEL_FILE_PATH);
    // createComputeContextThread.join();

    localWorkSize[0] = 16;
    localWorkSize[1] = 16;
    globalWorkSize[0] = 1024;
    globalWorkSize[1] = 1024;
}

void deinitOpenCl()
{
    // CL_CHECK(clFlush(commands));
    // CL_CHECK(clFinish(commands));

    // CL_CHECK(clReleaseKernel(kernel));
    // CL_CHECK(clReleaseProgram(program));
    CL_CHECK(clReleaseContext(context));
}
