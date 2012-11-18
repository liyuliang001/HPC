#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#define start_time clock_gettime(CLOCK_MONOTONIC, &start);
#define end_time clock_gettime(CLOCK_MONOTONIC, &finish);

struct timespec start, finish;

int main(int argc, char *argv[]){
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
    v1 = (float*)malloc(a_rows* a_cols * sizeof(float));
    v2 = (float*)malloc(a_cols* b_cols* sizeof(float));
    res = (float*)malloc(a_rows*b_cols* sizeof(float));

// read in data
    fread((void*)v1, sizeof(float), a_rows*a_cols, pfile);
    fread((void*)v2, sizeof(float), a_cols*b_cols, pfile);

    fclose(pfile);
    
// memset
    memset(res, 0.0, a_rows * b_cols * sizeof(float));
// set start
    start_time

// calculate
    int i, j, k;
    for(i=0; i<a_rows; i++)
        for(j=0; j<b_cols; j++)
            for(k=0; k<a_cols; k++)
                res[i*a_cols+j] += v1[i*a_cols+k] * v2[k*b_cols+j];

// set end
    end_time
    printf("%d %d %d %.16lf\n", a_rows, a_cols, b_cols, finish.tv_sec-start.tv_sec + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0);
   
    printf("a: %d %d \n", a_rows, a_cols);
    printf("b: %d %d \n", a_cols, b_cols);
    printf("c: %d %d \n", a_rows, b_cols);
#ifndef SILENT    
    for (i = 0; i < a_rows; i ++){
        for (j = 0; j < a_cols; j ++) 
            printf("%f ", v1[i*a_cols+j]);
            printf("\n");
    }
#endif 

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
}