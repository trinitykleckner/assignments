#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "read_ppm.h"

struct t_arg{
  int *array;
  struct ppm_pixel* image;
  struct ppm_pixel* palette;
  int size;
  int row;
  int col;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
};

struct ppm_pixel* make_one(struct ppm_pixel* pxls, struct ppm_pixel* palette, int size, int row, int col, float xmin, float xmax, float ymin, float ymax, int maxIterations);

void *thread_count(void* args) {
  struct t_arg * myargs = (struct t_arg *) args;
  int myid, i, row, col;
  myid = *((int*) args);

  // struct ppm_pixel* palette;
  // palette = malloc(sizeof(struct ppm_pixel)*myargs->maxIterations);
  // for(int i = 0; i < myargs->maxIterations; i++){
  //   palette[i].red = rand() % 255;
  //   palette[i].green = rand() % 255;
  //   palette[i].blue = rand() % 255;
  // }

  printf("Thread %lu) sub-image blocks: cols (%d,%d) to rows (%d,%d)\n", pthread_self(), myargs->row, myargs->size/2, myargs->col, myargs->size/2);
  make_one(myargs->image, myargs->palette, myargs->size, myargs->row, myargs->col, myargs->xmin, myargs->xmax, myargs->ymin, myargs->ymax, myargs->maxIterations);
  printf("Thread %lu) finished %d\n",pthread_self(), myid);
  return (void *)0;
}


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

  srand(time(0));
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  pthread_t threads[4];
  int ids[4];
  struct t_arg *thread_args = malloc(4*sizeof(struct t_arg));

  struct ppm_pixel* image;
  image = malloc(sizeof(struct ppm_pixel)*size*size);

  struct ppm_pixel* palette;
  palette = malloc(sizeof(struct ppm_pixel)*maxIterations);
  for(int i = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  for (int i=0; i<numProcesses; i++){
    thread_args[i].array = &ids[i];
    thread_args[i].palette = palette;
    thread_args[i].image = image;
    thread_args[i].size = size;
    thread_args[i].xmin = xmin;
    thread_args[i].xmax = xmax;
    thread_args[i].ymin = ymin;
    thread_args[i].ymax = ymax;
    thread_args[i].maxIterations = maxIterations;
    if(i < 2){thread_args[i].row = 0;}else{thread_args[i].row=size/2;}
    if(i % 2==0){thread_args[i].col = 0;}else{thread_args[i].col=size/2;}
  }

  for (int i = 0; i < numProcesses; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, thread_count, &thread_args[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%d x %d) in %f seconds\n",size, size, timer);
  char filename[100];
  sprintf(filename,"mandelbrot-%d-%ld.ppm",size,time(0));
  printf("writing file: %s\n",filename);
  write_ppm(filename, image, size, size);

  free(palette);
  palette = NULL;
  free(thread_args);
  thread_args = NULL;
  free(image);
  image = NULL;
  return 0;
}


struct ppm_pixel* make_one(struct ppm_pixel* pxls, struct ppm_pixel* palette, int size, int row, int col, float xmin, float xmax, float ymin, float ymax, int maxIterations){
  //creating color array
  struct ppm_pixel black;
  black.red = 0;
  black.green = 0;
  black.blue = 0;

  for(int i=0; i<size/2; i++){
    for(int j=0; j<size/2; j++){
      float xfrac =  (float)(i+row)/(float) size;
      float yfrac = (float)(j+col)/ (float) size;
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
        pxls[(i+row)+(j+col)*size].red = palette[iter].red;
        pxls[(i+row)+(j+col)*size].blue = palette[iter].blue;
        pxls[(i+row)+(j+col)*size].green = palette[iter].green;
      } else {
        pxls[(i+row)+(j+col)*size].red = black.red;
        pxls[(i+row)+(j+col)*size].green = black.green;
        pxls[(i+row)+(j+col)*size].blue = black.blue;
      }
    }
  }
  //free(palette);
  //palette = NULL;
  return pxls;
}
