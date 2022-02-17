#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

char getChar(float i){
  if(i < 26){return '@';}
  else if (i < 51){return '#';}
  else if (i < 76){return '%';}
  else if (i < 101){return '*';}
  else if (i < 126){return 'o';}
  else if (i < 151){return ';';}
  else if (i < 176){return ':';}
  else if (i < 201){return ',';}
  else if (i < 226){return '.';}
  else if (i < 256){return ' ';}
  else {return 1;}
}

int main(int argc, char** argv) {
  // todo
  char* filename;
  filename = argv[1];
  int w;
  int h;

  struct ppm_pixel* image = read_ppm(filename, &w, &h);

  for(int i=0; i<w*h; i++){
    float intensity;
    intensity = (image[i].red + image[i].green + image[i].blue)/3;
    //printf("%f\n",intensity);
    char character = getChar(intensity);
    printf("%c",character);
    if((i+1)%w == 0){
      printf("\n");
    }
  }

  free(image);
  image = NULL;
  return 0;
}
