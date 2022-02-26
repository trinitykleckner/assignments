#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  int rows;
  int cols;

  if(argc != 2){
    return 1;
  }

  char* filename = argv[1];

  struct ppm_pixel* image = read_ppm(filename,&rows,&cols);

  printf("Testing file %s: %d %d\n", filename, rows, cols);


  for(int i=0; i<rows*cols; i++){
    printf("(%u,%u,%u) ",image[i].red,image[i].green,image[i].blue);
    if((i+1)%cols == 0){
      printf("\n");
    }
  }

  free(image);
  return 0;
}
