# Matrix Library

This is a basic C++11 matrix library that supports the operations listed below:

* Scalar addition
* Matrix addition
* Scalar subtraction
* Matrix subtraction
* Scalar multiplication
* Matrix multiplication
* Scalar division

Experimental Features:
* OpenCL GPU acceleration
* OpenMP support

Current features in development:
* Cuda GPU acceleration
* Determinant calculation
* Matrix inverse computation
* Matrix division

# Building the library

To compile the matrix library in single threaded computation mode, follow these steps:
```
git clone https://github.com/bphun/MatrixLibrary.git
cd MatrixLibrary
mkdir build
cd build
cmake ../
make
```

To compile the matrix library in multi-threaded computation mode(using OpenMP), ensure that you have a compiler installed that supports OpenMP then follow these steps:
```
git clone https://github.com/bphun/MatrixLibrary.git
cd MatrixLibrary
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=<Path to compiler with OpenMP support> -DADDITIONS=OPENMP ../
make
```

To compile the matrix library in GPU computation mode(using OpenCL), ensure that you have a compiler installed that supports OpenMP then follow these steps:
```cmake
git clone https://github.com/bphun/MatrixLibrary.git
cd MatrixLibrary
mkdir build
cd build
cmake -DADDITIONS=OPENCL ../
make
```

# Usage
Once this matrix library is linked with your project, you can include the library in your source with ```#include <Matrix.h>```.

## Examples
Although all of the following examples use matrices that hold integers, this matrix library can be used with all numeric data types.

Instantiating an empty matrix:
```c++
Matrix<int> matrix;
```

Instantiating an empty matrix with a certain number of rows and columns:
```c++
const int rows = 1024;
const int columns = 1024;
Matrix<int> matrix(rows, columns)
```

Instantiating a matrix intialized with values of a numeric array:
```c++
const int rows = 2;
const int cols = 2;

int intArray[rows * cols] = {1, 2, 3, 4};
Matrix<int> matrix(intArray, rows, cols);
```

Instantiating a matrix intialized with values of a numeric vector:
```c++
const int rows = 2;
const int cols = 2;

vector<int> vector{1, 2, 3, 4};

Matrix<int> matrix(vector, rows, cols);
```

Adding two matrices:
```c++
const int rows = 2;
const int cols = 2;

int intArray[rows * cols] = {1, 2, 3, 4};

Matrix<int> matrixA(intArray, rows, cols);
Matrix<int> matrixB(intArray, rows, cols);

Matrix<int> &resultMatrix = matrixA + matrixB;
```

Multiplying a matrix by a scalar:
```c++
const int rows = 2;
const int cols = 2;

int intArray[rows * cols] = {1, 2, 3, 4};

Matrix<int> matrix(intArray, rows, cols);

Matrix<int> &resultMatrix = matrix * 2;
```

Multiplying two matrices:
```c++
const int rows = 2;
const int cols = 2;

int intArray[rows * cols] = {1, 2, 3, 4};

Matrix<int> matrixA(intArray, rows, cols);
Matrix<int> matrixB(intArray, rows, cols);

Matrix<int> &resultMatrix = matrixA * matrixB;
```

Subtracting two matrices:
```c++
const int rows = 2;
const int cols = 2;

int intArray[rows * cols] = {1, 2, 3, 4};

Matrix<int> matrixA(intArray, rows, cols);
Matrix<int> matrixB(intArray, rows, cols);

Matrix<int> &resultMatrix = matrixA - matrixB;
```

Dividing a matrix by a scalar:
```c++
const int rows = 2;
const int cols = 2;

int intArray[rows * cols] = {1, 2, 3, 4};

Matrix<int> matrix(intArray, rows, cols);

Matrix<int> &resultMatrix = matrix / 2;
```