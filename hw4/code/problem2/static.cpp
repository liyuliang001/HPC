// Usage: Static schedule implemented by MPI 

#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>

#define PI 3.1415926

int procs_id, procs_num;
int root_id = 0;

int std_sleep = 1000; // usec
int task_num = 40000;

int mode = 0;

int workload(int task_id, int mode){
	if(mode == 1){ // averagely
		return 1.5*std_sleep;
	}else if(mode == 2){ // incremendtal
		return std_sleep*(1+task_id/(double)task_num);
	}else if(mode == 3){ // sin 
//		return (sin(2*PI*task_id/task_num) + 1.5) * std_sleep;
		return std_sleep*(2-task_id/(double)task_num);
	}
}

void dummy(int usec){
	usleep(usec);
}

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procs_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_id);
	
	if(argc<3 && procs_id == root_id){
		printf("Format: mpirun -n $procs_num $bin $mode $std_sleep.\n");
		return 0;
	}
	
	mode = atoi(argv[1]);
	std_sleep = atoi(argv[2]);

	int bgn_id = 0 + procs_id*(task_num)/procs_num;
	int end_id = (procs_id+1)*task_num/procs_num;
	double time_start = MPI_Wtime();
	for(int i=bgn_id; i<end_id; i++){
		dummy(workload(i,mode));
	}
	MPI_Barrier(MPI_COMM_WORLD);
	double time_end = MPI_Wtime();

	if(procs_id == root_id){
		printf("Mode: %d Time: %lf\n", mode, (time_end-time_start));
	}

	MPI_Finalize();
	return 0;
}
