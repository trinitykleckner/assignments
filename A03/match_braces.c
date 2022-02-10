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
  newTop = top->next;
  free(oldTop);
  return newTop;
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  struct node* current = top;
  struct node* next;

  while(current != NULL){
    next = current->next;
    free(current);
    current = next;
  }
  top = NULL;
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  struct node* topCopy = top
  while(topCopy != NULL){
    printf("node with sym = %c, line = %d, col = %d",topCopy->sym,
    topCopy->linenum, topCopy->colnum);
    topCopy = topCopy->next;
  }
}

int checkFile(struct node* stack){

}

int main(int argc, char* argv[]) {
  FILE fp = fopen(argv[0],"r");
  if (fp == NULL) {
      printf("Unable to open file");
      return 1;
  }
  while(fgetc(fp))
  char thisChar = fgetc(fp);


  strut node* stack = (struct node*) malloc(sizeof(struct node));

  stack->next = NULL;
  return 0;
}
