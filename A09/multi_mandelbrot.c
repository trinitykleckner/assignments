#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"

struct ppm_pixel* make_one(struct ppm_pixel* pxls, int size, int rows, int cols, float xmin, float xmax, float ymin, float ymax, int maxIterations);

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
  struct timeval start, end;
  long mtime, secs, usecs;
  gettimeofday(&start, NULL);

  int shmid;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size * size, 0644 | IPC_CREAT);
  if (shmid == -1) {
     perror("Error: cannot initialize shared memory\n");
     exit(1);
   }

   struct ppm_pixel* image;
   image = shmat(shmid, NULL, 0);
   if(image == (void*) -1){
     perror("Error: cannot initialize image\n");
     exit(1);
   }

  pid_t child = fork();
  if(child > 0){
    printf("Launched child process %d\n",child);
    printf("%d) Sub-image block: cols (%d,%d) to rows (%d,%d)\n",child,0,size/2,0,size/2);
    pid_t child2 = fork();
    if(child2 > 0){
      printf("Launched child process %d\n",child2);
      printf("%d) Sub-image block: cols (%d,%d) to rows (%d,%d)\n",child2,size/2,size,0,size/2);
      pid_t child3 = fork();
      if(child3 > 0){
        printf("Launched child process %d\n",child3);
        printf("%d) Sub-image block: cols (%d,%d) to rows (%d,%d)\n",child3,0,size/2,size/2,size);
        pid_t child4 = fork();
        if (child4 > 0){
          printf("Launched child process %d\n",child4);
          printf("%d) Sub-image block: cols (%d,%d) to rows (%d,%d)\n",child4,size/2,size,size/2,size);
        } else if(child4 == 0){
          make_one(image, size, size/2, size/2, xmin, xmax, ymin, ymax, maxIterations);
          exit(0);
          printf("Child process complete: %d\n",child4);
        }
      } else {
        make_one(image, size, 0, size/2, xmin, xmax, ymin, ymax, maxIterations);
        exit(0);
        printf("Child process complete: %d\n",child3);
      }
    } else {
      make_one(image, size, size/2, 0, xmin, xmax, ymin, ymax, maxIterations);
      exit(0);
      printf("Child process complete: %d\n",child2);
    }
  } else {
    make_one(image, size, 0, 0, xmin, xmax, ymin, ymax, maxIterations);
    exit(0);
    printf("Child process complete: %d\n",child);
  }

  for(int i=0; i<4; i++){
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n",pid);
  }
 
   gettimeofday(&end, NULL);
   secs = end.tv_sec - start.tv_sec;
   usecs = end.tv_usec - start.tv_usec;
   mtime = ((secs)*1000 + usecs/1000.0) + 0.5;

   printf("Computed mandelbrot set (%d x %d) in %ld seconds\n",size, size, mtime); //((double)(t1-t0))/CLOCKS_PER_SEC);
  char filename[100];
  sprintf(filename,"multi-mandelbrot-%d-%ld.ppm",size,time(0));
  printf("writing file: %s\n",filename);
  write_ppm(filename, image, size, size);

  if(shmdt(image) == -1){
    perror("Error: cannot detatch shared memory\n");
    exit(1);
  }

  if(shmctl(shmid, IPC_RMID, 0) == -1){
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }


  return 0;
}


struct ppm_pixel* make_one(struct ppm_pixel* pxls, int size, int row, int col, float xmin, float xmax, float ymin, float ymax, int maxIterations){
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
  free(palette);
  palette = NULL;
  return pxls;
}
