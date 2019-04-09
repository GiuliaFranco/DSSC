#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>



double function(double x){
	return 1/(1+pow(x,2));
};

double serial_approx(int a,int b,double h,int N){
	double sum=0;
	int i=0;
	for(i=0;i<=N-1;i++){
                sum+=h*function(a+h*i+h/2);
        }
	return sum*4;
};


double local_sum(double local_a,double local_b,int local_n,double h){
	double local_result=0.0;
	int i;
	for(i=0;i<=local_n-1;++i){
		local_result+=h*function(local_a+h*i+h/2);
	}
	return local_result;

};

int reduction(int N){
	double global_result=0.0;
#pragma omp parallel reduction(+:global_result)
{
        double h=1/N;
        int tid=omp_get_thread_num();
        int nthreads=omp_get_num_threads();
        int local_n=N/nthreads;
        double local_a=local_n*tid*h;
        double local_b=local_a+h*local_n;
        global_result+=local_sum(local_a,local_b,local_n,h);
}
	return global_result*4;
};

int critical(int N){
        double global_result=0.0;
#pragma omp parallel 
{
        double h=1/N;
        int tid=omp_get_thread_num();
        int nthreads=omp_get_num_threads();
        int local_n=N/nthreads;
        double local_a=local_n*tid*h;
        double local_b=local_a+h*local_n;
	#pragma omp critical
        global_result+=local_sum(local_a,local_b,local_n,h);
}

        return global_result*4;
};


int atomic(int N){
        double global_result=0.0;
#pragma omp parallel
{
        double h=1/N;
        int tid=omp_get_thread_num();
        int nthreads=omp_get_num_threads();
        int local_n=N/nthreads;
        double local_a=local_n*tid*h;
        double local_b=local_a+h*local_n;
        double global=local_sum(local_a,local_b,local_n,h);
	#pragma omp atomic
	global_result+=global;
}

        return global_result*4;
};



int main( int argc, char * argv[] ) {
	double N=1000000000;
	double h=1/N;
	//printf("serial approximation:\n");
	//printf("%lf\n",serial_approx(0,1,h,N));
/*
	double final_duration=0.0;
	double tstart=0.0;
	tstart=omp_get_wtime();
	reduction(N);
	final_duration=omp_get_wtime()-tstart;
        printf("\t%lf\n",final_duration);
	
	double final_duration_c=0.0;
        double tstart_c=0.0;
        tstart_c=omp_get_wtime();
        critical(N);
        final_duration_c=omp_get_wtime()-tstart_c;
        printf("\t%lf\n",final_duration_c);	
*/
	double final_duration_a=0.0;
        double tstart_a=0.0;
        tstart_a=omp_get_wtime();
        atomic(N);
        final_duration_a=omp_get_wtime()-tstart_a;
        printf("\t%lf\n",final_duration_a);

	return 0;

}
