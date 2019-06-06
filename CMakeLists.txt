cmake_minimum_required(VERSION 3.13)

project(MatrixLibrary)

set(MatrixLibrary_VERSION_MAJOR 0)
set(MatrixLibrary_VERSION_MINOR 1)

add_compile_options(-Wall -std=c++11 -g -pthread)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

enable_testing()

set(PERFORMANCE_TEST_SRC "testing/MatrixLibraryPerformanceTests.cpp")
set(UNIT_TEST_SRC "testing/MatrixLibraryUnitTests.cpp")

if (DEFINED DEBUG)
    add_compile_definitions(DEBUG)
endif()

if (DEFINED ADDITIONS)
  if (${ADDITIONS} STREQUAL "OPENCL")
    message("GPU Mode")
    add_compile_definitions(ENABLE_OPENCL)
    #set(ADDITIONAL_SOURCES "src/OpenCL/ComputeMethod.cpp")
  endif()

  if(${ADDITIONS} STREQUAL "OPENMP")
    message("OpenMP Mode")
    add_compile_definitions(ENABLE_OPENMP)
    
    SET(CMAKE_CXX_FLAGS "-fopenmp")

    #set(ADDITIONAL_SOURCES "src/OpenMP/ComputeMethod.cpp")
  endif()
else()
  message("CPU Mode")
  #set(ADDITIONAL_SOURCES "src/SingleThreaded/ComputeMethod.cpp")
endif()

add_executable(matrixlibraryperformancetests ${PERFORMANCE_TEST_SRC} ${ADDITIONAL_SOURCES})
add_executable(matrixlibraryunittests ${UNIT_TEST_SRC} ${ADDITIONAL_SOURCES})

if (DEFINED ADDITIONS)
  if (${ADDITIONS} STREQUAL "OPENCL")
    add_compile_definitions(CL_SILENCE_DEPRECATION)

    find_package(OpenCL REQUIRED)
    include_directories(${OpenCL_INCLUDE_DIRS})
    link_directories(${OpenCL_LIBRARY})

    find_package (Threads)

    target_include_directories (matrixlibraryperformancetests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories (matrixlibraryunittests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    target_link_libraries(matrixlibraryperformancetests ${OpenCL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(matrixlibraryunittests ${OpenCL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
  endif()
  if (${ADDITIONS} STREQUAL "OPENCL")
  target_link_libraries(matrixlibraryperformancetests ${OpenMP_CXX_LIBRARIES})
  target_link_libraries(matrixlibraryunittests ${OpenMP_CXX_LIBRARIES})
  endif()
endif()

target_link_libraries(matrixlibraryunittests gtest_main gmock_main)
add_test(NAME matrixlibraryunittests COMMAND matrixlibraryunittests)