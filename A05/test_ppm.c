#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  int rows;
  int cols;
  // todo: call read_ppm
  //struct ppm_pixel* ptr =
  struct ppm_pixel* image = read_ppm("feep-raw.ppm",&rows,&cols);
  printf("Testing file feep-raw.ppm: %d %d\n", rows, cols);
  printf("%d\n",image[0].red );

  printf("%d\n",rows*cols);
  for(int i=0; i<rows*cols; i++){
    printf("here");
    printf("(%d,%d,%d) ",image[i].red,image[i].green,image[i].blue);
    if((i+1)%cols == 0){
      printf("\n");
    }  }

  free(image);
  return 0;
}
