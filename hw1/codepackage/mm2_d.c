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

    double *v1, *v2_tmp, **res;
    int a_rows, a_cols, b_cols;
    
// set size
    fread((void*)(&a_rows), sizeof(int), 1, pfile);
    fread((void*)(&a_cols), sizeof(int), 1, pfile);
    fread((void*)(&b_cols), sizeof(int), 1, pfile);
// alloc
    v1 = (double*)malloc(a_rows* a_cols * sizeof(double));
    v2_tmp = (double*)malloc(a_cols* b_cols* sizeof(double));
    res = (double**)malloc(a_rows * sizeof(double*));

    int i, j, k;
    for(i=0; i<a_rows; i++){
        res[i] = (double*)malloc(b_cols * sizeof(double));
        memset(res[i], 0.0, b_cols * sizeof(double));    
    }
    
// read in data
    fread((void*)v1, sizeof(double), a_rows*a_cols, pfile);
    fread((void*)v2_tmp, sizeof(double), a_cols*b_cols, pfile);

    fclose(pfile);
    
    double *v2;
    v2 = (double*)malloc(a_cols * b_cols * sizeof(double));
    for(i=0; i<a_cols; i++)
        for(j=0; j<b_cols; j++)
            v2[j*a_cols + i] = v2_tmp[i*b_cols+j];
    
// set start
    start_time

// calculate
    for(i=0; i<a_rows; i++)
        for(j=0; j<b_cols; j++)
            for(k=0; k<a_cols; k++){
//                     printf("%f", res[i][j]);
//                     printf("i: %d, j: %d, k: %d\n", i, j, k);
                res[i][j] += v1[i*a_cols+k] * v2[j*a_cols + k];
                
            }

// set end
    end_time
    printf("%d %d %d %.16lf\n", a_rows, a_cols, b_cols, finish.tv_sec-start.tv_sec + (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0);
   
    printf("a: %d %d \n", a_rows, a_cols);
    printf("b: %d %d \n", a_cols, b_cols);
    printf("c: %d %d \n", a_rows, b_cols);
#ifndef SILENT
    for (i = 0; i < a_rows; i ++){
        for (j = 0; j < a_cols; j ++) 
            printf("%lf ", v1[i*a_cols+j]);
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
        fwrite(res, sizeof(double), a_cols*b_cols, pfile);
        fclose(pfile);
    }
}