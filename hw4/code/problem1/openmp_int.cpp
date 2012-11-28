// Usage: Integral Using MPI
// Function: int(sin(x), a, b);

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <omp.h>

int msgtag;

int num_threads;
int trapezoid_num;
double a, b;

int main(int argc, char *argv[]){
	if(argc < 5){
		printf("Format: $bin $thread_num $trapezoid_num $a $b.\n");
		return 0;
	}
	printf("%s %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
	num_threads = atoi(argv[1]);
	a = atof(argv[2]);
	b = atof(argv[3]);
	trapezoid_num = atoi(argv[4]);

	omp_set_num_threads(num_threads);	
	
//	double time_start = MPI_Wtime();
	
	double step = (b-a)/trapezoid_num;
	double workload = (b-a)/(num_threads);
	double *bgn_idx = new double[num_threads];
	double *end_idx = new double[num_threads];
	for(int i=0; i<4; i++){
		bgn_idx[i] = i*workload;
		end_idx[i] = (i+1)*workload;
	}
	double *individual_sum = new double[num_threads];
//	double individual_sum[4];
	memset(individual_sum, 0, num_threads*sizeof(double));
	double total_sum = 0;

	double start_time = omp_get_wtime();
	double res=0;
#pragma omp parallel for reduction(+:res) 
	for(int i=0; i<trapezoid_num; i++){
		res += (sin(i*step)+sin(step*(i+1)))*step/2;
	}
/*#pragma omp parallel sections 
{
	#pragma omp section
	{
		double var;
		int thread_id = omp_get_thread_num();
		printf("Thread%d, bgn_idx: %lf, end_idx: %lf\n", thread_id, bgn_idx[thread_id], end_idx[thread_id]);
		for(var = bgn_idx[thread_id]; var <= end_idx[thread_id]; var = var + step)
			individual_sum[thread_id] += (sin(var)+sin(var+step))*step/2;
	}
	#pragma omp section
	{
		double var;
		int thread_id = omp_get_thread_num();
		printf("Thread%d, bgn_idx: %lf, end_idx: %lf\n", thread_id, bgn_idx[thread_id], end_idx[thread_id]);
		for(var = bgn_idx[thread_id]; var <= end_idx[thread_id]; var = var + step)
			individual_sum[thread_id] += (sin(var)+sin(var+step))*step/2;
	}
	#pragma omp section
	{
		double var;
		int thread_id = omp_get_thread_num();
		printf("Thread%d, bgn_idx: %lf, end_idx: %lf\n", thread_id, bgn_idx[thread_id], end_idx[thread_id]);
		for(var = bgn_idx[thread_id]; var <= end_idx[thread_id]; var = var + step)
			individual_sum[thread_id] += (sin(var)+sin(var+step))*step/2;
	}
	#pragma omp section
	{   
		double var;
		int thread_id = omp_get_thread_num();
		printf("Thread%d, bgn_idx: %lf, end_idx: %lf\n", thread_id, bgn_idx[thread_id], end_idx[thread_id]);
		for(var = bgn_idx[thread_id]; var <= end_idx[thread_id]; var = var + step)
			individual_sum[thread_id] += (sin(var)+sin(var+step))*step/2;
	}
}

	for(int i=0; i<num_threads; i++)
		total_sum += individual_sum[i];
*/
	double end_time = omp_get_wtime();

	printf("%lf %.16lf\n", (end_time-start_time), res);
	return 0;
}
