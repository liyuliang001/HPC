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

int shreshold;

int maxTaskNum = 40;
int verification_factor = 10;

typedef struct Task{
	int createdTime; 
	int lastTime; 
	int id;
	int executer_id;
	int status; // 1 for solution, 2 for verification, 3 for hangout
	void setTask(int _createdTime, int _lastTime, int _id, int _executer_id, int _status){
		createdTime = _createdTime;
		lastTime = _lastTime;
		id = _id;
		executer_id = _executer_id;
		status = _status;
	}
}Task;


vector<Task*> taskList;
int idleTime[5]={0};
int closed[5]={0};

int getWorkload(int i, int procs_id){ 
	double factor;
	switch(procs_id){
	case 1:
		factor = 1.1;
		break;
	case 2:
		factor = 1;
		break;
	case 3:
		factor = 0.8;
		break;
	case 4:
		factor = 0.5;
		break;
	default:
		factor = 1;
		break;
	}
	double res = (factor*(i*i+i+5));
	return (res<1)?1:res;
}
vector<Task*> taskPool;
void updateTaskPool(){
	for(int i=0; i<taskPool.size(); i++){
		for(int j=i+1; j<taskPool.size(); j++){
			if(taskPool[i]->lastTime > taskPool[j]->lastTime){
				Task* tmp;
				tmp = taskPool[i];
				taskPool[i] = taskPool[j];
				taskPool[j] = tmp;
			}
		}
	}
}
void initTaskPool(){
	for(int i=0; i<maxTaskNum; i++){
		Task* t = new Task;
		t->setTask(0, getWorkload(i, 0), i, 0, 1);
		taskPool.push_back(t);
	}
	updateTaskPool();
	for(int i=0; i<maxTaskNum; i++){
		printf("Init: %d %d\n", taskPool[i]->id, taskPool[i]->lastTime);
	}
}


// capability: procs4 > procs3 > procs2 > procs1
Task* getTask(int executer_id, int time){
	Task* t;

	updateTaskPool();
	
	int poolSize = taskPool.size();

	if(poolSize == 0){
		t = new Task;
		t->setTask(time, 0, 0, 0, 0);
		return t;
	}

	vector<Task*>::iterator iter;
	if(executer_id == 1 || executer_id == 2){
		int offset = (executer_id-1)*taskPool.size()/(procs_num-2);
		iter=taskPool.begin()+offset;
		while(iter!=taskPool.end()){
			if((*iter)->status==2 && (*iter)->executer_id==executer_id)
				iter++;
			else break;
		}
	}else if(executer_id == 3 || executer_id == 4){
		int offset = (procs_num-executer_id-1)*taskPool.size()/(procs_num-2);
		iter=taskPool.end()-1-offset; 
		while(iter!=taskPool.begin()){
			if((*iter)->status==2 && (*iter)->executer_id==executer_id)
				iter--;
			break;
		}
	}
	t = (*iter);
//	printf("time: %d, execter:%d, lastTime: %d\n", time, executer_id, getWorkload(t->id));
	if(executer_id <= 2 && getWorkload(t->id, executer_id) > shreshold && time > 3800){
		t = new Task;
		t->setTask(time, 1, t->id, executer_id, 3);
		return t;
	}
	
	
	if(t->status == 1){
		t->setTask(time, getWorkload(t->id, executer_id), t->id, executer_id, 1);
	//	printf("%d %d %d %d\n", time, t->id, executer_id, getWorkload(t->id, executer_id));
		Task *vt = new Task;
		vt->setTask(time, t->lastTime/10, t->id, executer_id, 2);
		taskPool.push_back(vt);
	}else if(t->status == 2){
		t->setTask(time, t->lastTime, t->id, executer_id, 2);
	}
	taskPool.erase(iter);

	taskList.push_back(t);
	return t;
}

void printTaskList(){
	for(int i=0; i<taskList.size(); i++){
		printf("%d %c ", taskList[i]->id, (taskList[i]->status==1)?'s':'v');
	}
	printf("\n");
}
FILE* pfile;
void print_results(int time){ 
	fprintf(pfile, "Total Study Time:%d\n", time);
//	printf("Param: %d %d\n", shreshold, time);
	fprintf(pfile, "Format: ($startTime, $timeSustained, $processId, $taskId, $(verification or solution)\n");
	for(vector<Task*>::iterator iter=taskList.begin(); iter!=taskList.end(); iter++){
		fprintf(pfile, "(%d, %d, %d, %d, %c)\n", (*iter)->createdTime, (*iter)->lastTime, (*iter)->executer_id, (*iter)->id, ((*iter)->status==1)?'s':'v');
	}

	for(int i=1; i<procs_num; i++){
		fprintf(pfile, "Process %d\n", i);
		for(int j=0; j<taskList.size(); j++){
			if(taskList[j]->executer_id == i){
				fprintf(pfile, "(%d, %c) ", taskList[j]->id, (taskList[j]->status==1)?'s':'v');
			}
		}
		fprintf(pfile, "\n");
	}
}

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procs_num);
	MPI_Comm_rank(MPI_COMM_WORLD, &procs_id);

	int msgtag;
	int time=0; // time for 1000Hz
	MPI_Status status;
	if(procs_id==root_id){

		initTaskPool();
		if(argc >= 3){
			pfile = fopen(argv[1], "w");
			shreshold = atoi(argv[2]);
		}else
			pfile = stdout;
		while(1){
			bool finished = true;			
			for(int i=1; i<procs_num; i++){
	//			printf("time: %d, idleTime[%d]: %d\n", time, i, idleTime[i]);
				if(time >= idleTime[i] && closed[i]==0){
					// Send task
	//				printf("root: getTask()\n");
					Task* t = getTask(i, time);
					MPI_Send(t, sizeof(Task)/sizeof(int), MPI_INT, i, msgtag, MPI_COMM_WORLD);
					idleTime[i] = t->createdTime + t->lastTime;
//					printf("Send to %d: last: %d, time: %d.\n", i, t->lastTime, time);
					if(t->status == 0){
						closed[i] = true;
					}
				}
			}

			time = time+1;

			// Sync
			for(int i=1; i<procs_num; i++){
				if(closed[i] == 0){
					finished=false;
					break;
				}
			}
//			printf("root: time:%d, finished: %d\n", time, finished);
			if(finished){ 
				print_results(time);
				fflush(pfile);

//				printf("Procs%d break on time %d\n", procs_id, time); 
				// broadcast to tell other processes to halt
				MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
				break;
			}
			else MPI_Barrier(MPI_COMM_WORLD);
		}
	}else{
		bool finished = false;
		while(1){
	//		printf("procs%d: come to %d\n", procs_id, time);
			if(time >= idleTime[procs_id] && finished == false){
				// Recv task
				Task* t=new Task;
				/*int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
				               MPI_Comm comm, MPI_Status *status)*/
				MPI_Recv(t, sizeof(Task)/sizeof(int), MPI_INT, root_id, msgtag, MPI_COMM_WORLD, &status);
				idleTime[procs_id] = t->createdTime + t->lastTime;
				/*
				   If recv a signal of close, then break;
				*/
				printf("Procs%d receive %d status %d on time: %d\n", procs_id, t->lastTime, t->status, time);
				if (t->status == 0){
//					printf("Procs%d break on time: %d.\n", procs_id, time);
					finished = true;
				}
			}
			time = time + 1;
			// Sync
			MPI_Barrier(MPI_COMM_WORLD);
		}
	}


	MPI_Finalize();
	return 0;
}
