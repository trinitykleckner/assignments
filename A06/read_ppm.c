#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an
// array of arrays
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *fp;
  fp = fopen(filename,"rb");
  if (fp == NULL) {
      printf("Unable to open file\n");
      return NULL;
  }

  char magicNumber[100];
  char c[100];
  int maxColor;

  fgets(magicNumber, sizeof(char)*100, fp);
  magicNumber[2] = '\0';
  //check for comment
  fgets(c, sizeof(char)*100, fp);
  if(c[0] == '#'){
    fgets(c, sizeof(char)*100, fp);
  }
  sscanf(c, " %d %d", w, h);
  fscanf(fp, " %d ", &maxColor);

  struct ppm_pixel* image;
  image = malloc((*w)*(*h)*sizeof(struct ppm_pixel));
  if (image == NULL){
    printf("malloc failed");
    return NULL;
  }
  fread(image, sizeof(struct ppm_pixel), (*h)*(*w), fp);

  fclose(fp);
  fp = NULL;
  return image;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h) {
  int j = 10;
  for(int i=0; i<w*h; i++){
    if(j%10==0){
      pxs[i].red = (pxs[i].red ^ pxs[i].green) <<(rand()%1);
      pxs[i].green = pxs[i].green && (rand()%10);
      pxs[i].blue = pxs[i].blue & pxs[i].red;
    } else {
      pxs[i].red = 0;
      pxs[i].green = (pxs[i].red ^ pxs[i].green) <<(rand()%1);
      pxs[i].blue = pxs[i].green && (rand()%10);
    }
    if(i%10 == 0){
      j++;
    }
  }


  FILE* fp;
  fp = fopen(filename,"wb");
  char* toAdd = malloc(sizeof(char)*1000);
  sprintf(toAdd, "P6\n%d %d\n255\n", w, h);
  fwrite(toAdd, sizeof(int), 4, fp);
  fwrite(pxs, sizeof(struct ppm_pixel), w*h, fp);

  free(toAdd);
  fclose(fp);
}
