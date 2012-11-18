#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#define start_time clock_gettime(CLOCK_MONOTONIC, &start);
#define end_time clock_gettime(CLOCK_MONOTONIC, &finish);

struct timespec start, finish;

int main(int argc, char *argv[]){
        char filepath[100];
        int a_rows=atoi(argv[1]), a_cols=atoi(argv[2]);
        sprintf(filepath, "../data/m2v/float_dataset/m%d_%d_to_v%d.float", a_rows, a_cols, a_cols);
        FILE* pfile = fopen(filepath, "r");
        float *v1, *v2, *res;

// set size
        fread((void*)(&a_rows), sizeof(int), 1, pfile);
        fread((void*)(&a_cols), sizeof(int), 1, pfile);

// alloc
        v1 = (float*)malloc(a_rows*a_cols * sizeof(float));

        v2 = (float*)malloc(a_cols * sizeof(float));
        res = (float*)malloc(a_cols * sizeof(float));

// read in data
        fread((void*)v1, sizeof(float), a_rows*a_cols, pfile);
        fread((void*)v2, sizeof(float), a_cols, pfile);
// memset
        memset(res, 0.0, a_rows * sizeof(float));

// set start
        start_time

// calculate
        int i, j;
        for(i=0; i<a_rows; i++)
                for(j=0; j<a_cols; j++)
                        res[i] += v1[i*a_cols+j] * v2[j];

// set end
        end_time
        printf("%d %d %.16lf\n", a_rows, a_cols, finish.tv_sec-start.tv_sec + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0);

// print out
#ifdef DEBUG
        for(i=0; i<a_rows; i++)
                printf("%d %d %f\n", a_rows, a_cols, res[i]);

        return 0;
#endif
}

