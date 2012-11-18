#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define start_time clock_gettime(CLOCK_MONOTONIC, &start);
#define end_time clock_gettime(CLOCK_MONOTONIC, &finish);

struct timespec start, finish;
	
int main(int argc, char *argv[]){
	char filepath[100];
	int size=atoi(argv[1]);
	sprintf(filepath, "../data/v2v/double_dataset/v%d_to_v%d.double", size, size);
//	printf("%s\n", filepath);
	FILE* pfile = fopen(filepath, "r");
	double *v1, *v2, *res;

// set size
	fread((void*)(&size), sizeof(int), 1, pfile);

// alloc
	v1 = (double*)malloc(size * sizeof(double));
	v2 = (double*)malloc(size * sizeof(double));
	res = (double*)malloc(size * sizeof(double));

// read in data
	fread((void*)v1, sizeof(double), size, pfile);
	fread((void*)v2, sizeof(double), size, pfile);

// set start
	start_time

// calculate
	int i;
	for(i=0; i<size; i++)
		res[i] = v1[i] * v2[i];	

// set end
	end_time
	printf("%d %.16lf\n", size, finish.tv_sec-start.tv_sec + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0);

// print out
#ifdef DEBUG
	for(i=0; i<size; i++)
		printf("%d %lf\n", size, res[i]);	

	return 0;
#endif
}

