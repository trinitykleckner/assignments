//---------------------------------------------------------------------
// sorted_snackbar.c
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
// Name:
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
    //creating the snack
    struct snack* thisNode = malloc(sizeof(struct snack));
    strcpy(thisNode->name, name);
    thisNode->quantity = quantity;
    thisNode->cost = cost;

    //adding it to the list
    if(snacks == NULL){
      snacks = thisNode;
      thisNode->next = NULL;
    } else {
      struct snack* checking = snacks;
      while(checking->next != NULL){
        if(strcmp(thisNode->name,checking->name) <= 0){ //need something for same name?
          thisNode->next = checking;
          snacks = thisNode;
          break;
        } else {
          checking = checking->next;
        }
      }
    }

  return snacks;
}

// Delete (e.g. free) all nodes in the given list of snacks
// Param snacks: the first node in the list (NULL if empty)
void clear(struct snack* snacks) {
  //while thisSnack != null?
}

int main() {
  int numOfSnacks;;
  printf("Enter a number of snacks: ");
  scanf("%d", &numOfSnacks);

  struct snack *head = NULL;
  head = malloc(sizeof(struct snack));
  if (head == NULL){
    printf("Malloc unsuccessful");
    return 1;
  }

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

    head = insert_sorted(head,thisName,thisQuant,thisCost);
    printf("%s\n",head->name );
  }

  printf("here %s",head->name);
  printItems(head);

  return 0;
}

void printItems(struct snack* list){
  struct snack* node = list;
  int counter = 0;
  while(node != NULL){
    printf("\n%d) %s \t cost: $%0.2f \t quantity: %d", counter,
    node->name, node->cost, node->quantity);
    node = node->next;
    counter ++;
  }
}
