#include <stdio.h>

int main() {
  char input[8];
  printf("Enter 8 bits: ");
  scanf("%s", input);

  int answer = 0;
  for(int i=0; i<8; i++){
    answer <<= 1;
    if(input[i] == '1'){
      answer += 1;
    }
  }
  printf("Your character is: %c\n",answer);
  return 0;
}
