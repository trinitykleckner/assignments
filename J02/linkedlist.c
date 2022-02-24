#include <stdio.h>
#include <string.h>

struct cake {
  float cost;
  char flavor[16];
  struct cake* next;
};

void cheapestCake(struct cake* first){
  struct cake* cheapest = first;
  while(first != NULL){
    printf("cake: %s ($%.2f)\n",first->flavor, first->cost);
    if(first->cost < cheapest->cost){
      cheapest = first;
    }
    first = first->next;
  }
  printf("The cheapest cake is %s\n",cheapest->flavor);
}

struct cake makeCake(const char* flavor, float cost) {
  struct cake newCake;
  strncpy(newCake.flavor, flavor, 16);
  newCake.cost = cost;
  newCake.next = NULL;
  return newCake;
}

// todo: implement cheapestCake

int main() {
  struct cake cake1 = makeCake("red velvet", 2.00);
  struct cake cake2 = makeCake("chocolate", 1.75);
  struct cake cake3 = makeCake("mocha", 3.50);
  // todo: define 4th cake
  struct cake cake4 = makeCake("vanilla",4.00);

  cake1.next = &cake2;
  cake2.next = &cake3;
  cake3.next = &cake4;
  // draw stack and heap here

  // todo: call cheapestCake
  cheapestCake(&cake1);
}
