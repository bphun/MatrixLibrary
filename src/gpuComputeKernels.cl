__kernel void matrixAddition(__global int* inputArrA, __global int* inputArrB, __global int* outputArr, int numCols)
{
   int tX = get_global_id(0); 
   int tY = get_global_id(1);

   outputArr[tY * numCols + tX] = inputArrA[tY * numCols + tX] + inputArrB[tY * numCols + tX];
}

__kernel void matrixSubtraction(__global int* inputArrA, __global int* inputArrB, __global int* outputArr, int numCols)
{
   int tX = get_global_id(0); 
   int tY = get_global_id(1);

   outputArr[tY * numCols + tX] = inputArrA[tY * numCols + tX] - inputArrB[tY * numCols + tX];
}

__kernel void scalarMultiplication(__global int* inputArr, __global int* outputArr, int scalar, int numCols)
{
   int tX = get_global_id(0); 
   int tY = get_global_id(1);

   outputArr[tY * numCols + tX] = inputArr[tY * numCols + tX] * scalar;
}

__kernel void scalarDivision(__global int* inputArr, __global int* outputArr, int scalar, int numCols)
{
   int tX = get_global_id(0); 
   int tY = get_global_id(1);

   outputArr[tY * numCols + tX] = inputArr[tY * numCols + tX] / scalar;
}
