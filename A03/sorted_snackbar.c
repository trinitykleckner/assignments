//---------------------------------------------------------------------
// sorted_snackbar.c
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name: Trinity Kleckner
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  int quantity;
  float cost;
  struct snack* next;
};

void printItems(struct snack* list);

// Insert a new node to a list (implemented as a linked list).
// The new node should store the given properties
// Param snacks: the first item in the list (NULL if empty)
// Param name: the snack name (max length is 32 characters)
// Param quantity: the snack quantity
// Param cost: the snack cost
// Returns the first item in the list

struct snack* insert_sorted(struct snack* snacks,
  const char* name, int quantity, float cost) {
    //creating a snack
    struct snack* thisNode = malloc(sizeof(struct snack));
    strcpy(thisNode->name, name);
    thisNode->quantity = quantity;
    thisNode->cost = cost;

    if(snacks->next == NULL){ //if snack goes at the begining
      snacks->next = thisNode;
      thisNode->next = NULL;
    } else {
      struct snack* prev = snacks;
      struct snack* checking = snacks->next;
      while(checking != NULL){
        if(strcmp(thisNode->name,checking->name) <= 0){ //if snack goes in the middle
          thisNode->next = checking;
          prev->next = thisNode;
          return snacks;
        } else {
          prev = prev->next;
          checking = checking->next;
        }
      }
      prev->next = thisNode;
      thisNode->next = NULL;
      return snacks;
    }
    return snacks;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
  struct snack* this = snacks->next;
  struct snack* next;

  while(this != NULL){
    next = this->next;
    free(this);
    this = next;
  }
  free(this);
  snacks->next = NULL;
}

int main() {
  int numOfSnacks;;
  printf("Enter a number of snacks: ");
  scanf("%d", &numOfSnacks);

  struct snack *snackList = NULL;
  snackList = malloc(sizeof(struct snack));
  if (snackList == NULL){
    printf("Malloc unsuccessful");
    return 1;
  }

  strcpy(snackList->name, "header");
  snackList->next = NULL;

  for(int i=0; i<numOfSnacks; i++){
    char thisName[32];
    float thisCost;
    int thisQuant;

    printf("\nEnter a name: ");
    scanf("%s", thisName);
    printf("\nEnter a cost: ");
    scanf("%g", &thisCost);
    printf("\nEnter a quantity: ");
    scanf("%d", &thisQuant);

    snackList = insert_sorted(snackList,thisName,thisQuant,thisCost);
  }

  printf("\n\nWelcome to my sorted snackbar!\n");
  //clear(snackList);
  if(snackList->next == NULL){
    printf("\nSorry, the snack bar is empty\n");
  } else {
    printItems(snackList);
  }
  free(snackList);
  snackList = NULL;
  return 0;
}

void printItems(struct snack* list){
  struct snack* node = list->next;
  int counter = 0;
  while(node->next != NULL){
    printf("\n%d) %s \t cost: $%0.2f \t quantity: %d", counter,
    node->name, node->cost, node->quantity);
    node = node->next;
    counter ++;
  }
  printf("\n%d) %s \t cost: $%0.2f \t quantity: %d\n", counter,
  node->name, node->cost, node->quantity);
}
