#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

void fillIdentity(int * A, int num_el,int rest,int rank,int N,int npes){
        int delta=0;
	int leng=num_el*N;
	for(int row = 0; row < num_el; row++){
		A[rank*num_el+row+delta]=1;
		delta+=N;		
	}
};

void fixRest(int * A, int rest,int num_el,int N,int npes){	
	int res=1;
	for(int row = 0; row < rest; row++){
                A[N*num_el*npes+res*N+row-rest]=1;
		res+=1;
        }

};

void fillmatrix(int * A, int N,int M){
	for(int row = 0; row < N*M; row++){
        	A[row]=0;
         }
};

void printMatrix(int * A, int N,int M){
   int col,row;
   int delta=0;
   for(row=0;row<N;row++){
   	for(col=0;col<M;col++){
                 printf("%d\t",A[col+delta]);
    	}
    delta+=N;
    printf("\n");
   }


};


int main( int argc, char * argv[] ){
  int rank=0,i,j,k,m,l;
  int npes=0;
  int N=atoi(argv[1]);
  int *mat = (int *)malloc(N*N * sizeof(int));
  fillmatrix(mat,N,N);
  printf("%d\n",N);
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  int num_el=N/npes;
  int leng=num_el*N;
  int rest=N%npes;
  int leng_rest=rest*N;
  int *local_mat = (int *)malloc(leng * sizeof(int));
  fillmatrix(local_mat,num_el,N);
  fillIdentity(local_mat,num_el,rest,rank,N,npes);
  MPI_Barrier(MPI_COMM_WORLD);
  printf("%d\n",rank);
  printMatrix(local_mat,num_el,N);
  MPI_Gather(local_mat,leng, MPI_INT, mat, leng, MPI_INT,0,MPI_COMM_WORLD);
  MPI_Finalize();
  printf("\n");
  if(rank==0) {
	if(rest>0) fixRest(mat,rest,num_el,N,npes);
	printMatrix(mat,N,N);
  }
  return 0;
}
