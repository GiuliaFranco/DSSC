#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
void print_usage( int * a, int N, int nthreads ) {

  int tid, i;
  for( tid = 0; tid < nthreads; ++tid ) {

    fprintf( stdout, "%d: ", tid );

    for( i = 0; i < N; ++i ) {

      if( a[ i ] == tid) fprintf( stdout, "*" );
      else fprintf( stdout, " ");
    }
    printf("\n");
  }
}

int main( int argc, char * argv[] ) {

	const int N=250;
	int a[N];
	int thread_id=0;
	int nthreads=10;
	int i=0;
	for(i=0;i<N;i++){
		a[i]=thread_id;
	}
	//print_usage(a,N,nthreads);
#pragma omp parallel private(thread_id)
{
	thread_id=omp_get_thread_num();
#pragma omp for schedule(static) private(i)
	for(i=0; i<N; i++){
                a[i]=thread_id;
        	}
}
	printf("print static:\n");
	print_usage(a,N,nthreads);


#pragma omp parallel private(thread_id)
{
        thread_id=omp_get_thread_num();
#pragma omp for schedule(static,1) private(i)
        for(i=0; i<N; i++){
                a[i]=thread_id;
                }
}
	printf("print static,chunk 1:\n");
        print_usage(a,N,nthreads);

#pragma omp parallel private(thread_id)
{
        thread_id=omp_get_thread_num();
#pragma omp for schedule(static,10) private(i)
        for(i=0; i<N; i++){
                a[i]=thread_id;
                }
}
	printf("print static,chunk 10:\n");
        print_usage(a,N,nthreads);


#pragma omp parallel private(thread_id)
{
        thread_id=omp_get_thread_num();
#pragma omp for schedule(dynamic) private(i)
        for(i=0; i<N; i++){
                a[i]=thread_id;
                }
}
        printf("print dynamic:\n");
        print_usage(a,N,nthreads);


#pragma omp parallel private(thread_id)
{
        thread_id=omp_get_thread_num();
#pragma omp for schedule(dynamic,1) private(i)
        for(i=0; i<N; i++){
                a[i]=thread_id;
                }
}
        printf("print dynamic,chunk 1:\n");
        print_usage(a,N,nthreads);

#pragma omp parallel private(thread_id)
{
        thread_id=omp_get_thread_num();
#pragma omp for schedule(dynamic,10) private(i)
        for(i=0; i<N; i++){
                a[i]=thread_id;
                }
}
        printf("print dynamic,chunk 10:\n");
        print_usage(a,N,nthreads);


	return 0;
}
