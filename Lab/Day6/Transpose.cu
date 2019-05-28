#include <stdio.h>
#include <assert.h>

const int DIM = 32;

// print GB/s
void postprocess(int n, float ms)
{

    printf("%21f\t", n * sizeof(double)*1e-6 / ms );
}
//Read the in matrix using pieces of 32 items.
// naive transpose
__global__ void transposeNaive(double *out,double *in,int BLOCK)
{
  int x = blockIdx.x * DIM + threadIdx.x;
  int y = blockIdx.y * DIM + threadIdx.y;
  int width = gridDim.x * DIM;

  for (int j = 0; j < DIM; j+= BLOCK)
    out[x*width + (y+j)] = in[(y+j)*width + x];   
//each thread executing transpose DIM/BLOCK_i elements from in column into out row. 
}


__global__ void transposeImproved(double *out, double *in,int BLOCK)
{
  __shared__ float tile[DIM][DIM];
    
  int x = blockIdx.x * DIM + threadIdx.x;
  int y = blockIdx.y * DIM + threadIdx.y;
  int width = gridDim.x * DIM;

  for (int j = 0; j < DIM; j += BLOCK)
     tile[threadIdx.y+j][threadIdx.x] = in[(y+j)*width + x];
//use shared memory in order to transpose the matrix and write back to out in row-wise.
  __syncthreads();  //needed in order to ensure that all the writes are performed.

  x = blockIdx.y * DIM + threadIdx.x;  // transpose block offset
  y = blockIdx.x * DIM + threadIdx.y;

  for (int j = 0; j < DIM; j += BLOCK)
     out[(y+j)*width + x] = tile[threadIdx.x][threadIdx.y + j];
}
   
void RunTest(int BLOCK,const int nx,const int ny,const int mem_size){
  dim3 dimGrid(nx/DIM, ny/DIM, 1);
  dim3 dimBlock(DIM, BLOCK, 1);
  printf("%d\t",DIM*BLOCK);
  
  double *h_in = (double*)malloc(mem_size);
  double *h_out = (double*)malloc(mem_size);

  double *d_in, *d_out;
  cudaMalloc(&d_in, mem_size);
  cudaMalloc(&d_out, mem_size);

  // host
  for (int j = 0; j < ny; j++)
    for (int i = 0; i < nx; i++)
      h_in[j*nx + i] = i;

  // device
  cudaMemcpy(d_in, h_in, mem_size, cudaMemcpyHostToDevice);

  // events for timing
  cudaEvent_t startEvent, stopEvent;
  cudaEventCreate(&startEvent);
  cudaEventCreate(&stopEvent);
  float ms;

  cudaMemset(d_out, 0, mem_size);
  cudaEventRecord(startEvent, 0);
  transposeNaive<<<dimGrid, dimBlock>>>(d_out, d_in,BLOCK);
  cudaEventRecord(stopEvent, 0);
  cudaEventSynchronize(stopEvent);
  cudaEventElapsedTime(&ms, startEvent, stopEvent);   //milliseconds
  printf("%21f\t",ms);
  cudaMemcpy(h_out, d_out, mem_size, cudaMemcpyDeviceToHost);
  postprocess(nx * ny, ms);


  cudaMemset(d_out, 0, mem_size); //Reset matrix so i don't have to allocate a new one
  cudaEventRecord(startEvent, 0);
  transposeImproved<<<dimGrid, dimBlock>>>(d_out, d_in,BLOCK);
  cudaEventRecord(stopEvent, 0);
  cudaEventSynchronize(stopEvent);
  cudaEventElapsedTime(&ms, startEvent, stopEvent);
  printf("%21f\t",ms);
  cudaMemcpy(h_out, d_out, mem_size, cudaMemcpyDeviceToHost);
  postprocess(nx * ny, ms);
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
  const int mem_size = nx*ny*sizeof(double);

  RunTest(BLOCK_1,nx,ny,mem_size);
  RunTest(BLOCK_2,nx,ny,mem_size);
  RunTest(BLOCK_3,nx,ny,mem_size);
}
