#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void* initial = sbrk(0);
  printf("The initial top of the heap is %p\n",initial);
  void* current = sbrk(0);
  printf("The current top of the heap is %p\n",current);
  unsigned int difference = current - initial;
  printf("Increased by %d (%#x) bytes\n", difference, difference);

  return 0;
}
