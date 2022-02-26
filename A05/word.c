#include <stdio.h>
#include <string.h>

int main() {
  char word[4];
  printf("Enter 4 characters: ");
  scanf("%s", word);

  unsigned int sum = 0;
  for(int i=0; i<4; i++){
    int thisIndex = 3-i;
    int shiftBy = 8*i;
    unsigned int a = (unsigned int)(word[thisIndex]<<shiftBy);
    sum += a;
    printf("Debug: %c = %#010X\n", word[thisIndex], a);
  }
  printf("Your number is: %d (%#010x)\n", sum, sum);

  return 0;
}
