#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  int rows;
  int cols;

  if(argc != 2){
    return 1;
  }

  char* filename = argv[1];
  char* newfilename = malloc(sizeof(int)*strlen(filename)+1);
  strncpy(newfilename, filename, strlen(filename)-4);
  strcat(newfilename, "-glitch.ppm");

  struct ppm_pixel* image = read_ppm(filename,&rows,&cols);
  printf("Reading %s with width %d and height %d\n", filename, rows, cols);

  write_ppm(newfilename,image,rows,cols);
  printf("Writing file %s\n", newfilename);

  free(newfilename);
  newfilename = NULL;
  free(image);
  image = NULL;

  return 0;
}
