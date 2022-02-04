#include <stdio.h>
#include <stdlib.h>

int main() {

  char* inpString = malloc(sizeof(char) * 32);
  int inpInt;

  if(inpString==NULL){
    printf("malloc unsuccessful");
    return 1;
  }


  printf("Enter a word: ");
  scanf("%s", inpString);
  printf("Enter a count: ");
  scanf("%d", &inpInt);

  printf("Your word is ");
  for(int i=0; i<inpInt; i++){
    printf("%s", inpString);
  }
  printf("\n");

  free(inpString);
  inpString = NULL;
  return 0;
}
