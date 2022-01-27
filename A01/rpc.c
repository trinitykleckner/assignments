#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* choiceToWord(int choice);
char* printOutcome(char aiChoice[9], char plyrChoice[9]);

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
  }
  //srand(time(0));
  return 0;
}

char *choiceToWord(int choice){
  char* options[3] = {"rock", "paper", "scissors"};
  for(int i = 0; i<3; i++){
    //int cmp = strcmp(options[i],choice);
    if(i == choice){
      return options[i];
    }
  }
  return "invalid choice";
}

char* printOutcome(char aiChoice[9], char plyrChoice[9]){
  printf("in printOutcome");
  return "end";
}
