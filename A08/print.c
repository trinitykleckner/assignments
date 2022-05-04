//#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//#include <time.h>

int main (int argc, char* argv[]) {
  void* initial = sbrk(0);
  printf("The initial top of the heap is %p\n",initial);
  void* current = sbrk(0);
  printf("The current top of the heap is %p\n",current);

  printf("Increased by %ld (%#lx) bytes\n", current - initial, current - initial);

  return 0;
}
