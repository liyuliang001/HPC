#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#define start_time clock_gettime(CLOCK_MONOTONIC, &start);
#define end_time clock_gettime(CLOCK_MONOTONIC, &finish);

struct timespec start, finish;

int main(int argc, char *argv[]){
        char filepath[100];
        int a_rows=atoi(argv[1]), a_cols=atoi(argv[2]), b_cols=atoi(argv[3]);
        sprintf(filepath, "../data/m2m/float_dataset/m%d_%d_to_m%d_%d.float", a_rows, a_cols, a_cols, b_cols);
        FILE* pfile = fopen(filepath, "r");
        float *v1, *v2, *res;

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
        
        


	/* output results */ 
	if (output_switch){
		if ((fp = fopen(output_file_name, "wb")) == NULL){
			printf("file: %s can not be opened \n", output_file_name);
			system("pause");
			return 0;
		}
		fwrite(c, DATA_SIZE, c_rows*c_cols, fp);
		fclose(fp); 
	}
}

