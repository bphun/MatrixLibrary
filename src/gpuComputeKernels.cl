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

__kernel void matrixMultiplication(__global int* inputArrA, __global int* inputArrB, __global int* outputArr, int numColsA, int numColsB)
{
   int tX = get_global_id(0); 
   int tY = get_global_id(1);

   int value = 0;
   for (int k = 0; k < numColsA; ++k)
   {
      int elementA = inputArrA[tY * numColsA + k];
      int elementB = inputArrB[k * numColsB + tX];
      value += elementA * elementB;
   }
 
   // Write the matrix to device memory each 
   // thread writes one element
   outputArr[tY * numColsB + tX] = value;
}

__kernel void scalarDivision(__global int* inputArr, __global int* outputArr, int scalar, int numCols)
{
   int tX = get_global_id(0); 
   int tY = get_global_id(1);

   outputArr[tY * numCols + tX] = inputArr[tY * numCols + tX] / scalar;
}
