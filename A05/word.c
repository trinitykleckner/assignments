#include <stdio.h>
#include <string.h>

int main() {
  char word[4];
  scanf("%s", word);
  printf("%s\n", word);


  for(int i=0; i<strlen(word); i++){
    printf("Debug: %c = %#08X\n",word[i],word[i]);
  }

  printf("%#X\n",'l'+'o'+'v'+'e' );
  return 0;
}
