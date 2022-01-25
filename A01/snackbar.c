#include <stdio.h>
#include <string.h>


struct snack {
  char name[50];
  float cost;
  int quant;
};

void printItems(struct snack arr[]);

int main() {
  struct snack snk1, snk2, snk3;

  strcpy(snk1.name, "Ice Cream");
  snk1.cost = 7.50;
  snk1.quant = 10;

  strcpy(snk2.name, "Reeses");
  snk2.cost = 1.75;
  snk2.quant = 7;

  strcpy(snk3.name, "Pretzels");
  snk3.cost = 5.25;
  snk3.quant = 3;

  struct snack snkArray[3] = {snk1, snk2, snk3};
  float money;
  int selection;

  printf("Welcome to my snack bar.\n");
  printf("\nHow much money do you have?  ");
  scanf("%g", &money);
  printItems(snkArray);
  printf("\nWhat would you like to buy? [0,1,2]  ");
  scanf("%d", &selection);
  if(snkArray[selection].cost > money){
    printf("Sorry, you do not have enough money to buy %s\n",snkArray[selection].name);
  } else {
    if(snkArray[selection].quant > 0){
      printf("\nYou bought %s\n", snkArray[selection].name);
      float newMoney = money - snkArray[selection].cost;
      printf("You have $%g left\n",newMoney);
    } else {
      printf("Sorry, there are no more %s in stock\n",snkArray[selection].name); 
    }
  }
  return 0;
}

void printItems(struct snack arr[3]){
  for(int i=0; i<3;i++){
    printf("\n%d) %s \t cost: $%f \t quantity: %d", i, arr[i].name, arr[i].cost, arr[i].quant);
  }
}
