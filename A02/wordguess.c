#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void guessingMod(char word[32]);
void printWord(char* word, int len);

int main(){
   int num;
   FILE *fp;
   char word[32];

   //opening the file
   fp = fopen("words.txt","r");
   if (fp == NULL){
       printf("Error! opening file");
       exit(1);
   }

   //picking a random word
   int numOfWords;
   fscanf(fp, "%d", &numOfWords);
   srand(time(0));
   int randNum = rand() % (numOfWords-2);
   randNum++; //so it never gets 0
      int i = 0;
   while(fgets(word,sizeof(char)*32,fp)){
     i++;
     if(i==randNum){
       printf("%s",word);
       break;
     }
   }

   guessingMod(word);
   fclose(fp);
   fp = NULL;

   return 0;
}

//the interactive part
void guessingMod(char word[32]){
  int turnCount = 1;
  int len = strlen(word)-1;
  char blank[len];

  for(int i=0; i<len; i++){
    blank[i] = '_';
  }

  printf("\nWelcome to Word Guess!\n");
  char guess[2];
  int counter = 0;
  while(counter < len){
    printf("\n\nTurn: %d\n\n",turnCount);
    printWord(blank,len);
    printf("\nGuess a character: ");
    scanf("%s",guess);
    int beforeCounter = counter;
    for(int i=0; i<len; i++){
      if((word[i] == guess[0])&&(blank[i]!=guess[0])){
        blank[i] = guess[0];
        counter++;
      }
    }
    if(beforeCounter == counter){
      printf("Sorry, this letter does not fill any blanks :(");
    }
    turnCount++;
  }

  printf("\n\n");
  printWord(word,len);
  printf("\nYou won in %d turns!!\n",turnCount);
}

void printWord(char* word, int len){
  for(int i=0; i<len; i++){
    printf("%c ",word[i]);
  }
}
