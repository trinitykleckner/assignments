#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

void buildSnack(struct snack* temp);
void printItems(struct snack* arr, int num);

int main() {
  int numOfSnacks;;
  printf("Enter a number of snacks: ");
  scanf("%d", &numOfSnacks);

  //initialize array to hold snacks
  struct snack* snkArr = malloc(sizeof(struct snack)*numOfSnacks);
  if(snkArr == NULL){
    printf("malloc unsuccessful");
    return 1;
  }

  for(int i=0; i<numOfSnacks; i++){
    struct snack* tempSnack = &snkArr[i];
    buildSnack(tempSnack);
  }

  printf("\n\nWelcome to my snackbar\n");  
  printItems(snkArr, numOfSnacks);
  printf("\n");

  free(snkArr);
  snkArr = NULL;
  return 0;
}

void buildSnack(struct snack* temp){
  char thisName[32];
  float thisCost;
  int thisQuant;

  printf("\nEnter a name: ");
  scanf("%s", thisName);
  printf("\nEnter a cost: ");
  scanf("%g", &thisCost);
  printf("\nEnter a quantity: ");
  scanf("%d", &thisQuant);

  strcpy(temp->name, thisName);
  temp->cost = thisCost;
  temp->quantity = thisQuant;
}

void printItems(struct snack* arr, int num){
  for(int i=0; i<num;i++){
    printf("\n%d) %s \t cost: $%0.2f \t quantity: %d", i, arr[i].name, arr[i].cost, arr[i].quantity);
  }
}
