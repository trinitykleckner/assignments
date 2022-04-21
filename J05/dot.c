#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000
static unsigned long long thread_dotproduct = 0;
pthread_mutex_t mutex;

struct t_arg{
  int *v;
  int *u;
  int start;
};

void *thread_dotprod(void* args) {
  struct t_arg * myargs = (struct t_arg *) args;
  int i;

  int thisprod = 0;
  for (int i = myargs->start; i < (myargs->start + (SIZE/4)); i++) {
    thisprod += myargs->u[i] * myargs->v[i];
  }

  pthread_mutex_lock(&mutex);
  thread_dotproduct += thisprod;
  pthread_mutex_unlock(&mutex);

  return (void *)0;
}



int main(int argc, char *argv[]) {
  srand(time(0));
  int dotproduct = 0;
  pthread_t threads[4];


  int v[SIZE];
  int u[SIZE];

  for (int i = 0; i < SIZE; i++) {
    v[i] = rand() % 1000 - 500;
    u[i] = rand() % 1000 - 500;
    dotproduct += u[i] * v[i];
  }
  printf("Ground truth dot product: %d\n", dotproduct);

  printf("Test with 4 threads\n");

  struct t_arg *thread_args = malloc(4*sizeof(struct t_arg));

  for(int i=0; i < 4; i++){
    thread_args[i].v = v;
    thread_args[i].u = u;
    thread_args[i].start = i*(SIZE/4);
  }

  pthread_mutex_init(&mutex, NULL);
  for (int i = 0; i < 4; i++) {
      pthread_create(&threads[i], NULL, thread_dotprod, &thread_args[i]);
  }

  for (int i = 0; i < 4; i++) {
      pthread_join(threads[i], NULL);
  }
  pthread_mutex_destroy(&mutex);

  // TODO: Implement your thread solution here
  printf("Answer with 4 threads: %lld\n",thread_dotproduct);



  free(thread_args);
  thread_args = NULL;
  return 0;
}
