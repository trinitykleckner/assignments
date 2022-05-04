#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  struct chunk *next;
  int inUse;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while(next != NULL){
    if(next->size >= size){
      if(prev != NULL){
        prev->next = next->next;
      } else {
        flist = next->next;
      }
      next->inUse = size;
      
      return (void*)(next+1);
    } else {
      prev = next;
      next = next->next;
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *)-1) {
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->inUse = size;
    return (void*)(cnk + 1);
  }
}

void free(void *memory) {
  if(memory != NULL){
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory -1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}

void fragstats(void* buffers[], int len) {
  //int used = 0;
  int free = 0;
  int eUtotal = 0;
  float eUaverage = 0;
  int eUbig = 0;
  int eUsmall = 999999999;
  int iUtotal = 0;
  float iUaverage = 0;
  int iUbig = 0;
  int iUsmall = 999999999;
  int chunkCount = 0;

  struct chunk *next = flist;

  while(next != NULL){
    free ++;
    struct chunk *cnkF = (struct chunk*)((struct chunk*)next);
    eUtotal += cnkF->size;
    if(cnkF->size > eUbig){
      eUbig = cnkF->size;
    }
    if(cnkF->size < eUsmall){
      eUsmall = cnkF->size;
    }
    next = next->next;
  }
  eUaverage = (float) eUtotal/free;

  for(int i=0; i<len; i++){
    if(buffers[i] != NULL){
      chunkCount ++;
      struct chunk *cnk = (struct chunk*)((struct chunk*)buffers[i]-1);
      iUtotal += cnk->inUse;
      if(cnk->inUse > iUbig){
        iUbig = cnk->inUse;
      }
      if(cnk->inUse < iUsmall){
        iUsmall = cnk->inUse;
      }
      iUaverage = (float) iUtotal/chunkCount;
    }
  }
  printf("Total blocks: %d Free: %d Used: %d\n",len+free, free, chunkCount);
  printf("Internal unused: total: %d average: %f smallest: %d largest: %d\n",iUtotal,iUaverage,iUsmall,iUbig);
  printf("External unused: total: %d average: %f smallest: %d largest: %d\n",eUtotal,eUaverage,eUsmall,eUbig);
}
