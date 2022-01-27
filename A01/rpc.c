#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* choiceToWord(int choice);
void printOutcome(char aiChoice[9], char plyrChoice[9]);
int getWinner(char aiChoice[9], char plyrChoice[9]);

int main() {
  int rounds;
  printf("Welcome to Rock, Paper, Scissors! \nHow many rounds do you want to play?  ");
  scanf("%d", &rounds);
  printf("%d", rounds);
  
  int aiScore = 0; 
  int plyrScore = 0;

  int counter = rounds; //why do I need this?
  for(int i=0; i < counter; i++){
   // printf("in the loop i = %d rounds = %d",i,rounds);
    char plyrChoice;
    int aiChoice = 0;
    char* aiChoiceWord;


    printf("Which do you choose? rock, paper, or scissors? ");
    scanf("%s", &plyrChoice);
    aiChoiceWord = choiceToWord(aiChoice);
    printf("AI chooses %s\n", aiChoiceWord);
    printOutcome(aiChoiceWord, &plyrChoice);
    int winner = getWinner(aiChoiceWord, &plyrChoice); 
  }
  //srand(time(0));
  return 0;
}

char *choiceToWord(int choice){
  char* options[3] = {"rock", "paper", "scissors"};
  for(int i = 0; i<3; i++){
    if(i == choice){
      return options[i];
    }
  }
  return "invalid choice";
}

void printOutcome(char aiChoice[9], char plyrChoice[9]){
  int cmp = strcmp(aiChoice, plyrChoice);
  if(cmp == 0){
    printf("It's a tie!\n");
  } else if(strcmp("rock",aiChoice)*strcmp("rock",plyrChoice) == 0) {
    if(strcmp("paper",aiChoice)*strcmp("paper",plyrChoice) == 0) {
      printf("Paper covers rock\n");
    } else {
      printf("Rock bashes scissors\n");
    }
  } else {
    printf("Scissors cut paper\n");
  }
}

int getWinner(char aiChoice[9], char plyrChoice[9]){
  return 0;
}
