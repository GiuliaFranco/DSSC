/*
 *
 * This code implements the first broadcast operation using the paradigm of message passing (MPI) 
 *
 * The broadcast operation is of kind 1:n where thre process root send a set of data to all other processes.
 * 
 * MPI_BCAST( buffer, count, datatype, root, comm )
 * [ INOUT buffer] starting address of buffer (choice)
 * [ IN count] number of entries in buffer (integer)
 * [ IN datatype] data type of buffer (handle)
 * [ IN root] rank of broadcast root (integer)
 * [ IN comm] communicator (handle)
 *
 * int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
 *
 * MPI_BCAST broadcasts a message from the process with rank root to all processes of the group, 
 * itself included. It is called by all members of group using the same arguments for comm, root. 
 * On return, the contents of root's communication buffer has been copied to all processes.
 *
 * The type signature of count, datatype on any process must be equal to the type signature of count, 
 * datatype at the root. This implies that the amount of data sent must be equal to the amount received, 
 * pairwise between each process and the root. 
 *
 * Detailed documentation of the MPI APis here:
 * https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node182.html
 *
 * Compile with: 
 * $mpicc bcast.c
 * 
 * Run with:
 * $mpirun -np 4 ./a.out
 *
 */

#include <stdlib.h>
#include <stdio.h>

// Header file for compiling code including MPI APIs
#include <mpi.h>


int main( int argc, char * argv[] ){
  int N=25000000;
  //int imesg = 0;
  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes
  int *imesg = (int *) malloc(sizeof(int) * 25000000);
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
    
  //imesg = rank;
  for(int i=0;i<25000000;i++){ 
	imesg[i]=-npes;
  }
  fprintf( stderr, "\nBefore Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, imesg[N-1] );
  //fprintf( stderr, "\nBefore Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, imesg );
  //just some processes will go into the broadcast. Since we have a collective call this will arise a deadblock.
  //if(rank % 2 ) MPI_Bcast( &imesg, 1, MPI_INT, 0, MPI_COMM_WORLD );
  MPI_Bcast( imesg, N, MPI_INT, 0, MPI_COMM_WORLD );
  //fprintf( stderr, "\nAfter Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, imesg );
  fprintf( stderr, "\nBefore Bcast operation I am process %d of %d MPI processes and my message content is: %d\n", rank, npes, imesg[N-1] );
  MPI_Finalize();
  free(imesg);
  return 0;

}
