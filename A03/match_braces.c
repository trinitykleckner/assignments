//---------------------------------------------------------------------
// match_braces.c
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name: Trinity Kleckner
//
#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};


// Push a new node to a stack (implemented as a linked list).
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  struct node* newNode = (struct node*) malloc(sizeof(struct node));
  newNode->sym = sym;
  newNode->linenum = line;
  newNode->colnum = col;
  newNode->next = top;
  return newNode;
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  struct node* oldTop = top;
  struct node* newTop = top->next;
  free(oldTop);
  return newTop;
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear2(struct node* top) {
  struct node* current = top;
  struct node* next;

  while(current != NULL){
    if(current==NULL){
      break;
    }
    next = current->next;
    printf("freeing %c",current->sym);
    free(current);
    current = next;
  }
  top = NULL;
}

void clear(struct node* top){
  struct node* this = top->next;
  struct node* next;
  while(this != NULL){
    next = this->next;
    free(this);
    this = next;
  }
  free(this);
  top = NULL;
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  struct node* topCopy = top;
  while(topCopy != NULL){
    printf("node with sym = %c, line = %d, col = %d",topCopy->sym,
    topCopy->linenum, topCopy->colnum);
    topCopy = topCopy->next;
  }
}

int main(int argc, char* argv[]) {
  //opening the file
  FILE *fp = fopen(argv[1],"r");
  if (fp == NULL) {
      printf("Unable to open file\n");
      return 1;
  }

  //creating the stack, and initializing first node to null
  struct node* stack = (struct node*) malloc(sizeof(struct node));
  if (stack == NULL) {
      printf("malloc unsuccessful");
      return 1;
  }
  stack = NULL;

  //iterating by char through the file, pushing and popping at {}
  int rowCounter = 1;
  int colCounter = 1;
  while(1){
    char thisChar = fgetc(fp);
    if(thisChar == EOF){
      break;
    }
    if(thisChar == '{'){
      stack = push(thisChar, rowCounter, colCounter, stack);
      //printf("%c\n",thisChar);
    } else if(thisChar == '}'){
        if(stack == NULL){
          printf("Unmatched brace on line %d, column %d\n",rowCounter,colCounter);
        } else {
          printf("Found matching braces: (%d, %d) -> (%d, %d)\n",
          stack->linenum, stack->colnum, rowCounter, colCounter);
          stack = pop(stack);
        }
    } else if(thisChar == '\n'){
      colCounter = 0;
      rowCounter++;
    }
    colCounter ++;
  }
  //clear(stack);
  //any braces left on the stack are unmatched
  while(stack != NULL){
    printf("Unmatched brace on line %d, column %d\n",stack->linenum,stack->colnum);
    stack = pop(stack);
  }

  free(stack);
  stack = NULL;
  return 0;
}
