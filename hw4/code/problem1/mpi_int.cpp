// Usage: Integral Using MPI
// Function: int(sin(x), a, b);

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>

int procs_id, procs_num;
int root_id = 0;
int msgtag;

int trapezoid_num;
double a, b;

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procs_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_id);
	
	if(argc < 4 && procs_id == root_id){
		printf("Format: $bin $procs_num $a $b $trapezoid_num.\n");
		return 0;
	}
	
	a = atof(argv[1]);
	b = atof(argv[2]);
	trapezoid_num = atoi(argv[3]);

	double time_start = MPI_Wtime();
	
	double step = (b-a)/trapezoid_num;
	double workload = (b-a)/(procs_num);
	double bgn_idx = procs_id*workload;
	double end_idx = (procs_id+1)*workload;
	double individual_sum = 0;
	double total_sum = 0;

	double var;
	for(var = bgn_idx; var <= end_idx; var = var + step){
		individual_sum += (sin(var)+sin(var+step))*step/2;
	}

	MPI_Reduce(&individual_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, root_id, MPI_COMM_WORLD);

	double time_end = MPI_Wtime();

	if(procs_id == root_id){
		printf("%lf %.16lf\n", (time_end-time_start), total_sum);
	}

	MPI_Finalize();
	return 0;
}
