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
int mode = 0;

int workload(int task_id, int mode){
	if(mode == 1){ // averagely
		return 1.5*std_sleep;
	}else if(mode == 2){ // incremendtal
		return std_sleep*(1+task_id/(double)task_num);
	}else if(mode == 3){ // sin 
		//return (sin(2*PI*task_id/task_num) + 1.5) * std_sleep;
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
	
	if(argc<4 && procs_id == root_id){
		printf("Format: mpirun -n $procs_num $bin $mode $std_sleep $chunk_size.\n");
		MPI_Finalize();
		return 0;
	}
	
	mode = atoi(argv[1]);
	std_sleep = atoi(argv[2]);
	chunk_size = atoi(argv[3]);
	

	int crnt_task_id = 0;
	int crnt_task_finished = 0;
	int *procs_state = new int[procs_num]; // 0 denotes idle, 1 busy, 2 closed
	memset(procs_state, 0, procs_num * sizeof(int));
	MPI_Status status;
	MPI_Request request;
	int msgtag;
	int *procs_task_id = new int[procs_num];
	memset(procs_task_id, 0, procs_num * sizeof(int));
	bool ever_send = false;

	double time_start = MPI_Wtime();

	if(procs_id == root_id){
		while(1){
			ever_send = false;
			for(int i=0; i<procs_num; i++){
				if(i == root_id) continue;
				else if(procs_state[i] == 0){
					if(procs_task_id[i] < task_num){ // Last time it was still working, I must send a task to let it know it should halt
						procs_task_id[i] = crnt_task_id; // If the crnt_task_id has exceeded task_num, I know that it has already known and halted. 
						MPI_Send(&crnt_task_id, 1, MPI_INT, i, msgtag, MPI_COMM_WORLD); 
						crnt_task_id += chunk_size;
						procs_state[i] = 1;
						ever_send = true;
					}
				}
			}
			for(int i=0; i<procs_num; i++){
				if(i == root_id) continue;
				else if(procs_state[i] == 1){
					if(procs_task_id[i] >= task_num)//{ printf("skip recving from procs %d.\n", i);}
						continue;
					MPI_Recv(&crnt_task_finished, 1, MPI_INT, i, msgtag, MPI_COMM_WORLD, &status);
					if(crnt_task_finished == 1)
						procs_state[i] = 0;
				}
			}
			if(ever_send == false) break;
		}
	}else{
		while(1){
			crnt_task_finished = 0;
			MPI_Recv(&crnt_task_id, 1, MPI_INT, root_id, msgtag, MPI_COMM_WORLD, &status);
			if(crnt_task_id >= task_num){ 
				break;
			}

			for(int i=crnt_task_id; i<crnt_task_id+chunk_size; i++){
				dummy(workload(i, mode));
			}
			crnt_task_finished = 1;
			MPI_Send(&crnt_task_finished, 1, MPI_INT, root_id, msgtag, MPI_COMM_WORLD);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	double time_end = MPI_Wtime();

	if(procs_id == root_id){
		printf("Mode: %d Time: %lf\n", mode, (time_end-time_start));
	}

	MPI_Finalize();
	return 0;
}
