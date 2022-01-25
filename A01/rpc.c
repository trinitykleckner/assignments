#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getWinner(char plyrChoice[9]);

int main() {
  int rounds;
  printf("Welcome to Rock, Paper, Scissors! \nHow many rounds do you want to play?  ");
  scanf("%d", &rounds);
  printf("%d\n", rounds);

  int aiScore = 0;
  int plyrScore = 0;

  for(int i=0; i<rounds; i++){
    char choice[9];
    printf("Which do you choose? rock, paper, or scissors?  ");
    scanf("%s", choice);
  }
  //srand(time(0));
  return 0;
}

int getWinner(char plyrChoice[9]){
  srand(time(0));
  int aiChoice;
}
