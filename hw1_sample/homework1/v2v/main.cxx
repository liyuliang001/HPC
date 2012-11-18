#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define DATA_TYPE float
#define DATA_SIZE sizeof(float)

int main(int argc, char **argv)
{
  char *input_file_name;
  char *output_file_name;
  FILE *fp;
  int  size;
  DATA_TYPE *a, *b;
  DATA_TYPE c;

  /* resolve the input arguments */
  if (argc < 2){
      printf("Error! the number of arguments is less(%d)!\n", argc);
      system("pause");
      return 0;
  }
  input_file_name = argv[1]; // get input file name

  /* read input data */
  if ((fp = fopen(input_file_name, "rb")) == NULL){
    printf("file: %s can not be opened \n", input_file_name);
    system("pause");
    return 0;
  }
  
  fread(&size, sizeof(int), 1, fp);
  a = new DATA_TYPE[size]; 
  b = new DATA_TYPE[size];
  
  memset(a, 0, DATA_SIZE*size);
  memset(b, 0, DATA_SIZE*size);
  fread(a, DATA_SIZE, size, fp);
  fread(b, DATA_SIZE, size, fp);
  fclose(fp);

  /* your homework programming */
  



  /* output result*/
  //printf("result: %f n", c);

  return 0;
}
