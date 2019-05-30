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

void Big_fixRest(int * A, int rest,int num_el,int N,int npes){
        int res=1;
        for(int row = 0; row < rest; row++){
                A[res*N+row-rest]=1;
                res+=1;
        }

};


void fillmatrix(int * A, int N,int M){
	for(int row = 0; row < N*M; row++){
        	A[row]=0;
         }
};

void printMatrix(int * A, int N,int M){
   printf("\n");
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

void swapBuffers(int** a, int** b)
{
    int * t = *a;
    *a = *b;
    *b = t;
}

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
  
  //If i have a small matrix it's reasonable to allocate all on a node.
  if(N<10){
  	MPI_Gather(local_mat,leng, MPI_INT, mat, leng, MPI_INT,0,MPI_COMM_WORLD);
  	if(rank==0) {
		printf("\n");
		if(rest>0) fixRest(mat,rest,num_el,N,npes);
		printMatrix(mat,N,N);
  	}
	free(local_mat);
	free(mat);
	MPI_Finalize();
  }

  //if the matrix is big then i need to overlap reading and writing
  if(N>=10){
	MPI_Status status;
  	MPI_Request send_request,recv_request;
	if(rank!=0){   MPI_Isend(local_mat,leng,MPI_INT,0,101,MPI_COMM_WORLD,&send_request);}
	if(rank==0){
		FILE* data_file;
                data_file=fopen("write_mat.txt","wb");

		//two buffers in order to overlap communications
		int * buffer1=local_mat;
		int * buffer2 = (int *)malloc(leng * sizeof(int));

		//first iteration is to write the  root values and receive the proc-1
		MPI_Irecv(buffer2,leng,MPI_INT,1,101,MPI_COMM_WORLD,&recv_request);
		//MPI_Wait(&recv_request, &status);
		fwrite(buffer1, sizeof(int), leng , data_file);
		MPI_Wait(&recv_request, &status);
		printMatrix(buffer1,num_el,N);

		
		int i;
		for(i=1;i<npes-1;i++){

			swapBuffers(&buffer2, &buffer1);
			MPI_Irecv(buffer2,leng,MPI_INT,i+1,101,MPI_COMM_WORLD,&recv_request);
                	fwrite(buffer1, sizeof(int), leng , data_file);
                	MPI_Wait(&recv_request, &status);
			printMatrix(buffer1,num_el,N);
		}

		fwrite(buffer2, sizeof(int), leng, data_file);
		printMatrix(buffer1,num_el,N);

		//add fixrest to the final result
		int * rest_buffer = (int *)malloc(leng_rest * sizeof(int));
		Big_fixRest(rest_buffer,rest,num_el,N,npes);
		fwrite(rest_buffer, sizeof(int), leng_rest, data_file);
		fclose(data_file);

		int* complete_mat = (int*) malloc(sizeof(int)*N*N);
		data_file = fopen("write_mat.txt","r");
		fread(complete_mat,sizeof(int),N*N,data_file);
		printf("\n");
		printMatrix(complete_mat,N,N);
		fclose(data_file);		
	}



	MPI_Finalize();


  }
  return 0;
}
