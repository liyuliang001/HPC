// Usage: Static schedule implemented by MPI 
// Recommended parameters: mpirun -n 4 ./guided 1 100 100 1.1505
#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <stack>
#include <memory.h>

#define PI 3.1415926

using namespace std;

int procs_id, procs_num;
int root_id = 0;

int std_sleep = 1000; // usec
int task_num = 40000;

int chunk_size = 0;
double shrink_factor = 0.0;
int mode = 0;

stack<int> steps;

int workload(int task_id, int mode){
	if(mode == 1){ // averagely
		return 1.5*std_sleep;
	}else if(mode == 2){ // incremendtal
		return std_sleep*(1+task_id/(double)task_num);
	}else if(mode == 3){ // sin 
		return (sin(2*PI*task_id/task_num) + 2) * std_sleep;
	}
}

void dummy(int usec){
	usleep(usec);
}

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procs_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_id);
	
	if(argc<4 && procs_id == root_id){
		printf("Format: mpirun -n $procs_num $bin $mode $std_sleep $chunk_size $shrink_factor.\n");
		MPI_Finalize();
		return 0;
	}
	
	mode = atoi(argv[1]);
	std_sleep = atoi(argv[2]);
	chunk_size = atoi(argv[3]);
	shrink_factor = atof(argv[4]);

	int tmp=task_num/procs_num;
	int step=chunk_size;
	while(tmp > 0){
		tmp = tmp-step;
		steps.push(step);
		if(tmp - step*shrink_factor <= 0) step = tmp;
		else step *= shrink_factor;
	}

	double time_start = MPI_Wtime();

	int task_idx = 0;
	while(!steps.empty()){
		for(int i=task_idx; i<task_idx+steps.top(); i++){
			dummy(workload(i, mode));
		}
		task_idx += procs_num * steps.top();
		steps.pop();
	}
	MPI_Barrier(MPI_COMM_WORLD);

	double time_end = MPI_Wtime();

	if(procs_id == root_id){
		printf("Mode: %d Time: %lf\n", mode, (time_end-time_start));
	}

	MPI_Finalize();
	return 0;
}
