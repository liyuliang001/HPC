#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define DATA_TYPE float
#define DATA_SIZE sizeof(float)

int main(int argc, char **argv)
{
  bool output_switch;  
  char *input_file_name;
  char *output_file_name;
  FILE *fp;
  int  a_rows, a_cols, b_rows, b_cols, c_rows, c_cols;
  int  i, offset;
  DATA_TYPE *buf1, *buf2, *buf3;
  DATA_TYPE **a, **b, **c;

  /* resolve the input arguments */
  if (argc < 2){
      printf("Error! the number of arguments is less(%d)!\n", argc);
      system("pause");
      return 0;
  }
  input_file_name = argv[1]; // get input file name

  output_switch = false;     // output results or not 
  if (argc > 2) {	    
      output_switch = true;
      output_file_name = argv[2];
  }
    
  if ((fp = fopen(input_file_name, "rb")) == NULL){
    printf("file: %s can not be opened \n", input_file_name);
    system("pause");
    return 0;
  }
  
  fread(&a_rows, sizeof(int), 1, fp);
  fread(&a_cols, sizeof(int), 1, fp);
  fread(&b_cols, sizeof(int), 1, fp); 
  b_rows = a_cols;
  c_rows = a_rows;
  c_cols = b_cols;

  buf1 = new DATA_TYPE[a_rows*a_cols];
  buf2 = new DATA_TYPE[b_rows*b_cols];
  buf3 = new DATA_TYPE[c_rows*c_cols];
  a = new DATA_TYPE *[a_rows]; 
  b = new DATA_TYPE *[b_rows];
  c = new DATA_TYPE *[c_rows];
  
  memset(buf1, 0, DATA_SIZE*a_rows*a_cols);
  memset(buf2, 0, DATA_SIZE*b_rows*b_cols);
  memset(buf3, 0, DATA_SIZE*c_rows*c_cols);
  fread(buf1, DATA_SIZE, a_rows*a_cols, fp);
  fread(buf2, DATA_SIZE, b_rows*b_cols, fp);
  fclose(fp);

  for (i = 0, offset = 0; i < a_rows; i ++, offset+=a_cols)
      a[i] = buf1 + offset;
  for (i = 0, offset = 0; i < b_rows; i ++, offset+=b_cols)
      b[i] = buf2 + offset;
  for (i = 0, offset = 0; i < c_rows; i ++, offset+=c_cols)
      c[i] = buf3 + offset;  

  /* your homework programming */
  printf("a: %d %d \n", a_rows, a_cols);
  printf("b: %d %d \n", b_rows, b_cols);
  printf("c: %d %d \n", c_rows, c_cols);
  for (int i = 0; i < a_rows; i ++){
     for (int j = 0; j < a_cols; j ++) 
	 printf("%f ", a[i][j]);
     printf("\n");
  }

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
   
  return 0;
}
