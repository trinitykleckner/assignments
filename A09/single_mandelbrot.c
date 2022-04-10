#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"


struct ppm_pixel* make_one(struct ppm_pixel* pxls, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations);

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate pallet
  // compute image
  srand(time(0));

  clock_t t0 = clock();

  struct ppm_pixel* image;
  image = malloc(sizeof(struct ppm_pixel)*size*size);
  image = make_one(image, size, xmin, xmax, ymin, ymax, maxIterations);

  clock_t t1 = clock();
  printf("Computed mandelbrot set (%d x %d) in %f seconds\n",size, size, (double)(t1-t0)/CLOCKS_PER_SEC);
  char filename[100];
  sprintf(filename,"multi-mandelbrot-%d-%ld.ppm",size,time(0));
  printf("writing file: %s\n",filename);
  write_ppm(filename, image, size, size);

  free(image);
  image = NULL;
}


struct ppm_pixel* make_one(struct ppm_pixel* pxls, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations){
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  // generate pallet

  //creating color array
  struct ppm_pixel black;
  black.red = 0;
  black.green = 0;
  black.blue = 0;

  struct ppm_pixel* palette;
  palette = malloc(sizeof(struct ppm_pixel)*maxIterations);
  for(int i = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){
      float xfrac =  (float)i/(float) size;
      float yfrac = (float) j/ (float) size;
      float x0 = xmin + xfrac*(xmax-xmin);
      float y0 = ymin + yfrac*(ymax-ymin);

      float x=0;
      float y=0;
      int iter = 0;
      while(iter < maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter ++;
      }
      if (iter < maxIterations){
        pxls[i*size+j].red = palette[iter].red;
        pxls[i*size+j].blue = palette[iter].blue;
        pxls[i*size+j].green = palette[iter].green;
      } else {
        pxls[i*size+j].red = black.red;
        pxls[i*size+j].green = black.green;
        pxls[i*size+j].blue = black.blue;
      }
    }
  }
  free(palette);
  palette = NULL;
  return pxls;
}
