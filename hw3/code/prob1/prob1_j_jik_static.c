#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#include <omp.h>

#define start_time clock_gettime(CLOCK_MONOTONIC, &start);
#define end_time clock_gettime(CLOCK_MONOTONIC, &finish);

struct timespec start, finish;

int main(int argc, char *argv[]){
    omp_set_num_threads(4);
    
    int output_switch; 
    char *input_file_name;
    char *output_file_name;
    FILE* pfile;

    /* resolve the input arguments */
    if (argc < 2){
        printf("Error! the number of arguments is less(%d)!\n", argc);
        system("pause");
        return 0;
    }
    input_file_name = argv[1]; // get input file name

    output_switch = 0;     // output results or not 
    if (argc > 2) {       
        output_switch = 1;
        output_file_name = argv[2];
    }

    if ((pfile = fopen(input_file_name, "rb")) == NULL){
        printf("file: %s can not be opened \n", input_file_name);
        system("pause");
        return 0;
    }

    float *v1, *v2, *res;
    int a_rows, a_cols, b_cols;
    
// set size
    fread((void*)(&a_rows), sizeof(int), 1, pfile);
    fread((void*)(&a_cols), sizeof(int), 1, pfile);
    fread((void*)(&b_cols), sizeof(int), 1, pfile);
// alloc

    v1 = (double*)malloc(a_rows* a_cols * sizeof(double));
    v2 = (double*)malloc(a_cols* b_cols* sizeof(double));
    res = (double*)malloc(a_rows*b_cols* sizeof(double));

// read in data
    fread((void*)v1, sizeof(double), a_rows*a_cols, pfile);
    fread((void*)v2, sizeof(double), a_cols*b_cols, pfile);

// memset
    memset(res, 0.0, a_rows * b_cols * sizeof(double));

// set start
    start_time

// calculate
    int i, j, k;
    
#pragma omp parallel private(i, j, k)
    {
#pragma omp for schedule(static)
    for(j=0; j<b_cols; j++)
    	for(i=0; i<a_rows; i++){
            for(k=0; k<a_cols; k++)
                res[i*a_cols+j] += v1[i*a_cols+k] * v2[k*b_cols+j];
    }
    }
// set end
    end_time
    printf("%d %d %d %.16lf\n", a_rows, a_cols, b_cols, finish.tv_sec-start.tv_sec + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0);

// print out
/*#ifdef DEBUG*/
/*    for(i=0; i<a_rows; i++)*/
/*        printf("%d %d %f\n", a_rows, a_cols, res[i]);*/
    /* output results */ 
    if (output_switch){
        if ((pfile = fopen(output_file_name, "wb")) == NULL){
            printf("file: %s can not be opened \n", output_file_name);
            system("pause");
            return 0;
        }
        fwrite(res, sizeof(float), a_cols*b_cols, pfile);
        fclose(pfile);
    }
    return 0;
}

