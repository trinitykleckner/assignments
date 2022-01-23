#include <stdio.h>
#include <string.h>

char *makePass(char inp[100]) {
  for(int i=0; inp[i]!='\0'; i++){
    if(inp[i] == 'e'){
      inp[i] = '3';
    } else if(inp[i] == 'l'){
      inp[i] = '1';
    } else if(inp[i] == 'a'){
      inp[i] = '@';
    } 
  }
  return inp;
}

int main() {
  char inp[100];
  printf("Enter a word: ");
  scanf("%s", inp);

  char *outp = makePass(inp);
  printf("%s\n",inp);
  return 0;
}

