#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

int main( int argc, char * argv[] ){
  int rank=0,i,j;
  int npes=0;
  MPI_Status status;
  MPI_Request send_request,recv_request;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  int N=10;
  int * vect=(int *)malloc(sizeof(int)*N);
  int * rec_vect=(int *)malloc(sizeof(int)*N);
  srand(time(NULL));
   for(i=0;i<N;i++){
     vect[i]=rand() % 30;
     rec_vect[i]=0;
   }
  for(i=0;i<npes-1;i++){
   if(rank == i){MPI_Isend(vect,N,MPI_INT,i+1,101,MPI_COMM_WORLD,&send_request);}
   if(rank ==i+1){  
    MPI_Irecv(rec_vect,N, MPI_INT,i,101,MPI_COMM_WORLD,&recv_request);	 
    MPI_Wait(&recv_request,&status);
    for(j=0;j<N;j++){vect[j]+=rec_vect[j];}
   }
  }
  
  MPI_Bcast(vect,N, MPI_INT,3, MPI_COMM_WORLD);
  for(i=0;i<N;i++){ printf("%d\n",vect[i]);}
  MPI_Finalize();
  return 0;
}
