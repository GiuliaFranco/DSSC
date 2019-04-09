#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <mpi.h>

int main( int argc, char * argv[] ){
  long int N=1000000000;
  double x,h,sum,mypi,total_sum;
  int rank=0,i;
  int npes=0;
  double start = MPI_Wtime();
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  h   = 1.0 / (double) N;
  for (i = rank + 1; i <= N; i += npes) {
	    x = h * ((double)i - 0.5);
	    sum += 4.0 / (1.0 + x*x);
	}
  mypi = h * sum;
  MPI_Reduce(&mypi,&total_sum,1,MPI_DOUBLE,MPI_SUM,npes-1,MPI_COMM_WORLD);
  double end = MPI_Wtime();
  
  if(rank==npes-1) MPI_Send(&total_sum,1,MPI_DOUBLE,0,101,MPI_COMM_WORLD);
  if(rank==0){
	 MPI_Recv(&total_sum,1, MPI_DOUBLE,npes-1,101,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	 printf("\t%lf\n",total_sum);
  }
  MPI_Finalize();
  printf("%d",npes);
  printf("\t%lf\n",end - start);

  
  return 0;
}
