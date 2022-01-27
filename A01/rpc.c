#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int checkPlayerInput(char* input);
char* choiceToWord(int choice);
char* printOutcome(char aiChoice[9], char plyrChoice[9]);
int getWinner(char aiChoice[9], char plyrChoice[9]);
void printWinner(int aiScore, int plyrScore);

int main() {
  srand(time(0));
  int rounds;
  printf("Welcome to Rock, Paper, Scissors! \nHow many rounds do you want to play?  ");
  scanf("%d", &rounds);
  
  int aiScore = 0; 
  int plyrScore = 0;

  int counter = rounds; //why do I need this?
  for(int i=0; i < counter; i++){
   // printf("in the loop i = %d rounds = %d",i,rounds);
    char plyrChoice[9];
    int aiChoice = rand() % 3;
    char* aiChoiceWord;


    printf("Which do you choose? rock, paper, or scissors? ");
    scanf("%s", plyrChoice);
    if(checkPlayerInput(plyrChoice) == 1){
      printf("invalid input- you must choose rock, paper, or scissors\n");
      return 0;
    }
    aiChoiceWord = choiceToWord(aiChoice);
    printf("AI chooses %s\n", aiChoiceWord);

    char* winner = printOutcome(aiChoiceWord, plyrChoice);
    if(strcmp(aiChoiceWord, winner) == 0){
      aiScore ++;
    } else if(strcmp(plyrChoice, winner)==0){
      plyrScore++;
    }

    printf("AI score: %d, Player score: %d\n\n", aiScore, plyrScore);
  }
  printWinner(aiScore, plyrScore);
  return 0;
}


int checkPlayerInput(char* input){
  if(strcmp(input,"rock")*strcmp(input,"paper")*strcmp(input,"scissors") == 0){
    return 0;
  }
  return 1;
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

char* printOutcome(char aiChoice[9], char plyrChoice[9]){
  int cmp = strcmp(aiChoice, plyrChoice);
  if(cmp == 0){
    printf("It's a tie!\n");
    return "tie";
  } else if(strcmp("rock",aiChoice)*strcmp("rock",plyrChoice) == 0) {
    if(strcmp("paper",aiChoice)*strcmp("paper",plyrChoice) == 0) {
      printf("Paper covers rock\n");
      return "paper";
    } else {
      printf("Rock bashes scissors\n");
      return "rock";
    }
  } else {
    printf("Scissors cut paper\n");
    return "scissors";
  }
}

void printWinner(int aiScore, int plyrScore){
  if(aiScore == plyrScore){
    printf("its a tie!\n");
  } else if(aiScore > plyrScore){
    printf("AI wins\n");
  } else if(aiScore < plyrScore){
    printf("You win!\n");
  }
}
