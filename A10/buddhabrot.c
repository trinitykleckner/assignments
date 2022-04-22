#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include "read_ppm.h"

static unsigned long maxCount = 0;
pthread_mutex_t mutex;

struct t_arg{
  int *array;
  struct ppm_pixel* image;
  struct ppm_pixel* palette;
  int* member;
  int* counts;
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


void *detMembership(void* args){
  struct t_arg * this = (struct t_arg *) args;
  int i, row, col;

  printf("Thread %lu) sub-image blocks: cols (%d,%d) to rows (%d,%d)\n", pthread_self(), this->row, this->row+this->size/2, this->col, this->col+this->size/2);

  for(int i=0; i<this->size/2; i++){
    for(int j=0; j<this->size/2; j++){
      float xfrac =  (float)(i+this->row)/(float) this->size;
      float yfrac = (float)(j+this->col)/ (float) this->size;
      float x0 = this->xmin + xfrac*(this->xmax-this->xmin);
      float y0 = this->ymin + yfrac*(this->ymax-this->ymin);

      float x=0;
      float y=0;
      int iter = 0;
      while(iter < this->maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter ++;
      }

      if(iter < this->maxIterations){
        this->member[(i+this->row)*this->size+(j+this->col)] = 0;
      } else {
        this->member[(i+this->row)*this->size+(j+this->col)] = 1;
      }
    }
  }
  return (void *)0;
}

void *compVisits(void* args){
  struct t_arg * this = (struct t_arg *) args;
  int i, row, col;
  int allCount = 0;

  for(int i=0; i<this->size/2; i++){
    for(int j=0; j<this->size/2; j++){
      if(this->member[(i+this->row)*this->size+(j+this->col)] == 1){
        float xfrac =  (float)(i+this->row)/(float) this->size;
        float yfrac = (float)(j+this->col)/ (float) this->size;
        float x0 = this->xmin + xfrac*(this->xmax-this->xmin);
        float y0 = this->ymin + yfrac*(this->ymax-this->ymin);

        float x=0;
        float y=0;
        while(x*x + y*y < 2*2){
          float xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;

          float yrow = this->size*(y-this->ymin)/(this->ymax-this->ymin); //need to round this
          float xcol = this->size*(x-this->xmin)/(this->xmax-this->xmin); //and this
          if(yrow > 0 || yrow < this->size){
            if(xcol > 0 || xcol < this->size){
                this->counts[(j+this->row)+(i+this->col)*this->size] ++;
                allCount ++;
                //printf("%d\n",allCount);
            }
          }
        }
      }
    }
  }
  //printf("%ld\n",pthread_self());
  //printf("\n\n\n %d\n\n",allCount);

  pthread_mutex_lock(&mutex);
  maxCount += allCount;
  pthread_mutex_unlock(&mutex);
  return (void *)0;
}

void *compColors(void* args){
  struct t_arg * this = (struct t_arg *) args;
  int myid, i, row, col;
  myid = *((int*) args);
  float gamma = 0.681;
  float factor = 1/gamma;
  for(int i=0; i<this->size/2; i++){
    for(int j=0; j<this->size/2; j++){
      double value = 0;
      //printf("%d\n",(j+this->row)+(i+this->col)*this->size);
      if(this->counts[(i+this->row)*this->size+(j+this->col)] > 0){
        printf("HERE");
        value = log(this->counts[(i+this->row)*this->size+(j+this->col)])/log(maxCount);
        value = pow(value,factor);
      }
      this->image[(i+this->row)*this->size+(j+this->col)].red = value*255;
      this->image[(i+this->row)*this->size+(j+this->col)].blue = value*255;
      this->image[(i+this->row)*this->size+(j+this->col)].green = value*255;
      //printf("here- %f",value*255);
    }
  }
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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);


  srand(time(0));
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  pthread_t threads[4];
  int ids[4];
  struct t_arg *thread_args = malloc(4*sizeof(struct t_arg));

  struct ppm_pixel* image = malloc(sizeof(struct ppm_pixel)*size*size);
  int* member = malloc(sizeof(int)*size*size);
  int* counts = malloc(sizeof(int)*size*size);

  struct ppm_pixel* palette;
  palette = malloc(sizeof(struct ppm_pixel)*maxIterations);
  for(int i = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  for(int i=0; i<numProcesses; i++){
    thread_args[i].array = &ids[i];
    thread_args[i].image = image;
    thread_args[i].palette = palette;
    thread_args[i].member = member;
    thread_args[i].counts = counts;
    thread_args[i].size = size;
    thread_args[i].xmin = xmin;
    thread_args[i].xmax = xmax;
    thread_args[i].ymin = ymin;
    thread_args[i].ymax = ymax;
    thread_args[i].maxIterations = maxIterations;
    if(i < 2){thread_args[i].row = 0;}else{thread_args[i].row=size/2;}
    if(i % 2 == 0){thread_args[i].col = 0;}else{thread_args[i].col=size/2;}
    //thread_args[i].row = 0;
    //thread_args[i].col = 0;
  }

  pthread_mutex_init(&mutex, NULL);
  for(int i=0; i<numProcesses; i++){
    ids[i] = i;
    pthread_create(&threads[i], NULL, detMembership, &thread_args[i]);
    pthread_create(&threads[i], NULL, compVisits, &thread_args[i]);
    pthread_create(&threads[i], NULL, compColors, &thread_args[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }
  pthread_mutex_destroy(&mutex);

  gettimeofday(&tend, NULL);
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%d x %d) in %f seconds\n",size, size, timer);
  char filename[100];
  sprintf(filename,"buddhabrot-%d-%ld.ppm",size,time(0));
  printf("writing file: %s\n",filename);
  write_ppm(filename, image, size, size);

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
  return pxls;
}
