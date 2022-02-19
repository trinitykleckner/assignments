#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"


// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an
// array of arrays
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *fp;
  fp = fopen(filename,"r");
  if (fp == NULL) {
      printf("Unable to open file\n");
      return NULL;
  }

  char magicNumber[100];
  char c[100];
  //char comment[100];
  int maxColor;

  fgets(magicNumber, sizeof(char)*100, fp);
  magicNumber[2] = '\0';
  //check for comment
  fgets(c, sizeof(char)*100, fp);
  if(c[0] == '#'){
    fgets(c, sizeof(char)*100, fp);
  }
  sscanf(c, " %d %d", w, h);
  fscanf(fp, " %d", &maxColor);
  //as  printf("magic number: %s\nrows: %d\ncols: %d\n",magicNumber,*w,*h);

  struct ppm_pixel* image = malloc((*w)*(*h)*sizeof(struct ppm_pixel));
  if (image == NULL){
    printf("malloc failed");
    return NULL;
  }
  for(int r=0; r<*h; r++){
    for(int c=0; c<*w; c++){
      struct ppm_pixel thisPixel;
      fscanf(fp, " %hhd %hhd %hhd", &thisPixel.red, &thisPixel.green, &thisPixel.blue);
      image[r*(*w)+c] =thisPixel;
    }
  }
  //printf("magic number: %s\nrows: %d\ncols: %d\n",magicNumber,*w,*h);

  fclose(fp);
  return image;
}
