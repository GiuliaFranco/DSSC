#include <stdio.h>
#include <assert.h>

const int DIM = 32;

// print GB/s
void postprocess(int n, float ms)
{

    printf("%21f\t", n * sizeof(double)*1e-6 / ms );   //can be multiplied by 2 -> once for reading the matrix and the other
                                                       //for writing.
}
//Read the in matrix using pieces of 32 items.
// naive transpose
__global__ void transposeNaive(double *out,double *in,int BLOCK)
{
  int x = blockIdx.x * DIM + threadIdx.x;
  int y = blockIdx.y * DIM + threadIdx.y;
  
  for (int j = 0; j < DIM; j+= BLOCK)
    out[x*(gridDim.x * DIM) + (y+j)] = in[(y+j)*(gridDim.x * DIM) + x];   
//each thread executing transpose DIM/BLOCK_i elements from in column into out row. 
}


__global__ void transposeImproved(double *out, double *in,int BLOCK)
{
  __shared__ double aux_mat[DIM][DIM];
    
  int x = blockIdx.x * DIM + threadIdx.x;
  int y = blockIdx.y * DIM + threadIdx.y;
  
  for (int j = 0; j < DIM; j += BLOCK)
     aux_mat[threadIdx.y+j][threadIdx.x] = in[(y+j)*(gridDim.x * DIM) + x];
//use shared memory in order to transpose the matrix and write back to out in row-wise.
  __syncthreads();  //needed in order to ensure that all the writes are performed.

  x = blockIdx.y * DIM + threadIdx.x;  // transpose block offset
  y = blockIdx.x * DIM + threadIdx.y;

  for (int j = 0; j < DIM; j += BLOCK)
     out[(y+j)*(gridDim.x * DIM) + x] = aux_mat[threadIdx.x][threadIdx.y + j];
}
   
void RunTest(int BLOCK,const int nx,const int ny,const int size){
  dim3 dimGrid(nx/DIM, ny/DIM, 1);
  dim3 dimBlock(DIM, BLOCK, 1);
  printf("%d\t",DIM*BLOCK);
  
  double *h_in = (double*)malloc(size);
  double *h_out = (double*)malloc(size);

  double *d_in, *d_out;
  cudaMalloc(&d_in, size);
  cudaMalloc(&d_out, size);

  // host
  for (int j = 0; j < ny; j++)
    for (int i = 0; i < nx; i++)
      h_in[j*nx + i] = i;

  // device
  cudaMemcpy(d_in, h_in, size, cudaMemcpyHostToDevice);

  // events for timing
  cudaEvent_t startEvent, stopEvent;
  cudaEventCreate(&startEvent);
  cudaEventCreate(&stopEvent);
  float time_m;

//time measures might contain overhead due to kernel lauch     
  cudaMemset(d_out, 0, size);
  cudaEventRecord(startEvent, 0);
  transposeNaive<<<dimGrid, dimBlock>>>(d_out, d_in,BLOCK);
  cudaEventRecord(stopEvent, 0);
  cudaEventSynchronize(stopEvent);
  cudaEventElapsedTime(&time_m, startEvent, stopEvent);   //milliseconds
  printf("%21f\t",time_m);
  cudaMemcpy(h_out, d_out, size, cudaMemcpyDeviceToHost);
  postprocess(nx * ny, time_m);


  cudaMemset(d_out, 0, size); //Reset matrix so i don't have to allocate a new one
  cudaEventRecord(startEvent, 0);
  transposeImproved<<<dimGrid, dimBlock>>>(d_out, d_in,BLOCK);
  cudaEventRecord(stopEvent, 0);
  cudaEventSynchronize(stopEvent);
  cudaEventElapsedTime(&time_m, startEvent, stopEvent);
  printf("%21f\t",time_m);
  cudaMemcpy(h_out, d_out, size, cudaMemcpyDeviceToHost);
  postprocess(nx * ny, time_m);
  printf("\n");

  // cleanup
  cudaEventDestroy(startEvent);
  cudaEventDestroy(stopEvent);
  cudaFree(d_out);
  cudaFree(d_in);
  free(h_in);
  free(h_out);


}
int main(int argc, char **argv)
{
 printf("# Threads  Naive Transpose(ms)  Naive Bandwidth (GB/s)   Improved Transpose(ms)  Improved Bandwidth (GB/s)\n");
//it's convenient to have num of threads < than elements in a submatrix.
  
  int BLOCK_1 = 2;   //each thread transpose DIM/BLOCK_i elements in the matrix.
  int BLOCK_2 = 16;
  int BLOCK_3 = 32; 				//each block transpose a submatrix of DIMxDIM size 
  const int nx = 8192;
  const int ny = 8192;
  const int size = nx*ny*sizeof(double);

  RunTest(BLOCK_1,nx,ny,size);
  RunTest(BLOCK_2,nx,ny,size);
  RunTest(BLOCK_3,nx,ny,size);
}
