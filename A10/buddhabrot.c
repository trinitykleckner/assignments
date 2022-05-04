#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "read_ppm.h"

static unsigned long maxCount = 0;
pthread_mutex_t mutex;
pthread_barrier_t barrier;


struct t_arg {
    struct ppm_pixel * image;
    struct ppm_pixel * palette;
    bool * member;
    int * count;
    int rStart;
    int cStart;
    int rEnd;
    int cEnd;
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    int maxIterations;
    int size;
};

void * startRoutine(void * inputData){
    struct t_arg *this = (struct t_arg *) inputData;

    //alg 1
    for (int i = this->rStart; i < this->rEnd; i++) {
        for (int j = this->cStart; j < this->cEnd; j++) {
            float xfrac = i / (float) this->size;
            float yfrac = j / (float) this->size;
            float x0 = this->xmin + xfrac * (this->xmax - this->xmin);
            float y0 = this->ymin + yfrac * (this->ymax - this->ymin);

            float x = 0;
            float y = 0;
            int iter = 0;

            while (iter < this->maxIterations && (x * x + y * y < 2 * 2)) {
                float xtmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtmp;

                iter++;
            }

            if (iter < this->maxIterations) {
                this->member[i * this->size + j] = false;
            } else {
                this->member[i * this->size + j] = true;
            }
        }
    }

    //alg 2
    for (int i = this->rStart; i < this->rEnd; i++) {
        for (int j = this->cStart; j < this->cEnd; j++) {
            if (this->member[i * this->size + j] == false) {
                float xfrac = i / (float) this->size;
                float yfrac = j / (float) this->size;
                float x0 = this->xmin + xfrac * (this->xmax - this->xmin);
                float y0 = this->ymin + yfrac * (this->ymax - this->ymin);

                float x = 0;
                float y = 0;
                int iter = 0;

                while (x*x + y*y < 2*2) {
                    float xtmp = x*x - y*y + x0;
                    y = 2*x*y + y0;
                    x = xtmp;
                    iter ++;

                    int yrow = round(this->size * (y - this->ymin) / (this->ymax - this->ymin));
                    int xcol = round(this->size * (x - this->xmin) / (this->xmax - this->xmin));

                    if (yrow < 0 || yrow >= this->size) continue; // out of range
                    if (xcol < 0 || xcol >= this->size) continue; // out of range

                    pthread_mutex_lock(&mutex);
                    this->count[yrow * this->size + xcol] += 1;
                    pthread_mutex_unlock(&mutex);

                    pthread_mutex_lock(&mutex);
                    if (maxCount < this->count[yrow * this->size + xcol]) {
                        maxCount = this->count[yrow * this->size + xcol];
                    }
                    pthread_mutex_unlock(&mutex);
                }
            }
        }
    }
    pthread_barrier_wait(&barrier);

    //alg 3
    double gamma = 0.681;
    double factor = 1.0/gamma;
    for (int i = this->rStart; i < this->rEnd; i++) {
        for (int j = this->cStart; j < this->cEnd; j++) {
            double value = 0;
            if (this->count[i * this->size + j] > 0){
                value = log(this->count[i * this->size + j])/log(maxCount);
                value = pow(value, factor);
            }
            this->image[i * this->size + j].red = value * 255;
            this->image[i * this->size + j].blue = value * 255;
            this->image[i * this->size + j].green = value * 255;
        }
    }
    return NULL;
}


int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.5;
  float xmax = 1.0;
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


  srand(time(0));
  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  pthread_t threads[4];
  struct t_arg thread_args[4];

  struct ppm_pixel*image = (struct ppm_pixel *) malloc(size*size*sizeof(struct ppm_pixel));
  bool* member = malloc(sizeof(bool)*size*size);
  int* count = malloc(sizeof(int)*size*size);
  memset(count, 0, size * size * sizeof(int));

  struct ppm_pixel* palette;
  palette = malloc(sizeof(struct ppm_pixel)*maxIterations);
  for(int i = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  pthread_barrier_init(&barrier,NULL,4);
  pthread_mutex_init(&mutex,NULL);

    for (int i = 0; i < numProcesses; i++) {
        thread_args[i].image = image;
        thread_args[i].palette = palette;
        thread_args[i].xmin = xmin;
        thread_args[i].xmax = xmax;
        thread_args[i].ymin = ymin;
        thread_args[i].ymax = ymax;
        thread_args[i].maxIterations = maxIterations;
        thread_args[i].size = size;
        thread_args[i].member = member;
        thread_args[i].count = count;
        if (i == 0){
            thread_args[i].rStart = 0;
            thread_args[i].rEnd = size/2;
            thread_args[i].cStart = 0;
            thread_args[i].cEnd = size/2;
        } else if (i == 1){
            thread_args[i].rStart = 0;
            thread_args[i].rEnd = size/2;
            thread_args[i].cStart = size/2;
            thread_args[i].cEnd = size;
        } else if (i == 2){
            thread_args[i].rStart = size/2;
            thread_args[i].rEnd = size;
            thread_args[i].cStart = 0;
            thread_args[i].cEnd = size/2;
        } else {
            thread_args[i].rStart = size/2;
            thread_args[i].rEnd = size;
            thread_args[i].cStart = size/2;
            thread_args[i].cEnd = size;
        }
      }
    for(int i = 0; i < numProcesses; i++){
      pthread_create(&threads[i], NULL, startRoutine, (void*) &thread_args[i]);
      printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
             (int) threads[i], thread_args[i].cStart, thread_args[i].cEnd, thread_args[i].rStart, thread_args[i].rEnd);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d) finished\n", (int) threads[i]);
    }

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Computed buddhabrot set (%dx%d) in %g seconds\n", size, size, timer);

    char filename[100];
    sprintf(filename, "buddhabrot-%d-%ld.ppm", size, time(0));

    write_ppm(filename, image, size, size);
    printf("\nWriting file %s\n", filename);

    free(image);
    image = NULL;
    free(palette);
    palette = NULL;
    free(count);
    count = NULL;
    free(member);
    member = NULL;
}
