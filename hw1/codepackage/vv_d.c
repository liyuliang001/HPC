#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    int size;
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

#ifndef SILENT
    for (i = 0; i < a_rows; i ++){
        printf("%f ", v1[i]);
    }
    printf("\n");
#endif

    /* output results */ 
    if (output_switch){
        if ((pfile = fopen(output_file_name, "wb")) == NULL){
            printf("file: %s can not be opened \n", output_file_name);
            system("pause");
            return 0;
        }
        fwrite(res, sizeof(double), size, pfile);
        fclose(pfile);
    }
}

