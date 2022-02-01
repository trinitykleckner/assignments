#include <stdio.h>

int main() {
//need to use malloc and free
  char inpString[32];
  int inpInt;

  printf("Enter a word: ");
  scanf("%s", inpString);
  printf("Enter a count: ");
  scanf("%d", &inpInt);

  printf("Your word is ");
  for(int i=0; i<inpInt; i++){
    printf("%s", inpString);
  }

  return 0;
}
