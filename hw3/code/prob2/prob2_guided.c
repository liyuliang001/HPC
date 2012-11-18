#include <stdio.h>
#include <time.h>

#include <omp.h>
#include <stdlib.h>

#define start_time clock_gettime(CLOCK_MONOTONIC, &start);
#define end_time clock_gettime(CLOCK_MONOTONIC, &finish);

struct timespec start, finish;

void dummy(int m_sec){
	usleep(m_sec);
}

int main(int argc, char* argv[]){
	
	int i;
	int thread_num = atoi(argv[1]);
	int iter_num = atoi(argv[2]);
	int m_sec = atoi(argv[3]);

	omp_set_num_threads(thread_num);

	start_time
#pragma omp parallel for schedule(guided, 100)
	for(i=0; i<iter_num; i++){
		dummy(i*m_sec);
	}
	end_time
	printf("Thread Number: %d, Dummy Time: %d msec, Number of Iteration: %d, Schedule: Guided, Time consumed: %.16lf\n", thread_num, m_sec, iter_num, finish.tv_sec - start.tv_sec + (double)(finish.tv_nsec-start.tv_nsec)/1000000000.0);

	return 0;
}
