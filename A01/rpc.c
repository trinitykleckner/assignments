#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int checkPlayerInput(char* input);
char* choiceToWord(int choice, char* options[]);
char* printOutcome(char aiChoice[9], char plyrChoice[9]);
int getWinner(char aiChoice[9], char plyrChoice[9]);
void printWinner(int aiScore, int plyrScore);

int main() {
  srand(time(0));
  char* options[3] = {"rock","paper","scissors"};

  int aiScore = 0;
  int plyrChoice = 0;

  int rounds;
  printf("Welcome to Rock, Paper, Scissors! \nHow many rounds do you want to play?  ");
  scanf("%d", &rounds);

  //one loop for each round
  int counter = rounds; 
  for(int i=0; i < counter; i++){
    char plyrChoice[9];
    int aiChoice = rand() % 3;
    char* aiChoiceWord;

    //getting player choice
    printf("Which do you choose? rock, paper, or scissors? ");
    scanf("%s", plyrChoice);
    if(checkPlayerInput(plyrChoice) == 1){
      printf("invalid input- you must choose rock, paper, or scissors\n");
      return 0;
    }

    //getting ai choice
    aiChoiceWord = choiceToWord(aiChoice, options);
    printf("AI chooses %s\n", aiChoiceWord);

    //finding and printing results
    char* winner = printOutcome(aiChoiceWord, plyrChoice);
    if(strcmp(aiChoiceWord, winner) == 0){
      aiScore ++;
    } else if(strcmp(plyrChoice, winner)==0){
      plyrScore++;
    }

    //update score 
    printf("AI score: %d, Player score: %d\n\n", aiScore, plyrScore);
  }

  printWinner(aiScore, plyrScore);
  return 0;
}

//assures the player picked a valid input
int checkPlayerInput(char* input){
  if(strcmp(input,"rock")*strcmp(input,"paper")*strcmp(input,"scissors") == 0){
    return 0;
  }
  return 1;
}

//turns the randint into rock, paper, or scissors
char *choiceToWord(int choice, char* options[]){
  for(int i = 0; i<3; i++){
    if(i == choice){
      return options[i];
    }
  }
  return "invalid choice";
}

//prints the outcome of the round, and returns the winning choice
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

//prints who wins the whole game (highest score)
void printWinner(int aiScore, int plyrScore){
  if(aiScore == plyrScore){
    printf("its a tie!\n");
  } else if(aiScore > plyrScore){
    printf("AI wins\n");
  } else if(aiScore < plyrScore){
    printf("You win!\n");
  }
}
