#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main( int argc, char *argv[] ){

  int myid = atoi( argv[1] );
  double var = (double) myid;

  if( myid % 2 == 0 ){

    printf("\nI am %d and I am an even process\n", myid);
    printf("\nResult of my function is: %g\n", exp( var ) );
  }
  else{
    printf("\nI am %d and I am an odd process\n", myid);
    printf("\nResult of my function is: %g\n", exp( var ) );
  }

  return 0;
}
