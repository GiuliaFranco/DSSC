#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
void swapPointers(int** a, int** b)
{
    int* t = *a;
    *a = *b;
    *b = t;
};

int main( int argc, char * argv[] ){
  int rank=0,i,j,k;
  int npes=0;
  MPI_Status status;
  MPI_Request send_request,recv_request;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  int N=4;
  int * vect=(int *)malloc(sizeof(int)*N);
  int* sum = malloc(sizeof(int)*N);
  int * rec_vect=(int *)malloc(sizeof(int)*N);
  srand(time(NULL));
   for(i=0;i<N;i++){
     vect[i]=rand() % 30;
     rec_vect[i]=0;
   }
 

for(k=0;k<npes;k++){
                if(rank==k){
		printf("initial value of process ");
                printf("%d\n",rank);
                printf("\n");
                for(i=0;i<N;i++){ printf("%d\n",vect[i]);}
        }
}

//All processes sums locally what received and send.
for(i=0;i<npes;i++){
   	MPI_Isend(vect,N,MPI_INT,(rank+1)%npes,101,MPI_COMM_WORLD,&send_request);  
	for(j=0;j<N;j++){sum[j]+=vect[j];}
	MPI_Wait(&send_request,&status);
	MPI_Irecv(rec_vect,N, MPI_INT,(rank-1+npes)%npes,101,MPI_COMM_WORLD,&recv_request);
	MPI_Wait(&recv_request,&status);
	swapPointers(&rec_vect,&vect);
   }
  
for(k=0;k<npes;k++){
	if(rank==k){
		printf("final value of process ");
		printf("%d\n",rank);
                printf("\n");
  		for(i=0;i<N;i++){ printf("%d\n",sum[i]);}
  	}
  }
  printf("\n");
  MPI_Finalize();
  return 0;
}
