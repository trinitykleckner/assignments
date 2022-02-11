//---------------------------------------------------------------------
// magic_square.c
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Trinity Kleckner
//
#include <stdio.h>
#include <stdlib.h>

//int checkRows(int nrows, int ncols, int ptr, int sum);
//int checkCols(int nrows, int ncols, int ptr, int sum);


int main() {
  int rows;
  int cols;
  scanf(" %d %d", &rows, &cols);

  if(rows*cols == 1){
    int onlyVal;
    scanf(" %d", &onlyVal);
    printf("%d\nthis matrix is magical!!! its constant is %d\n",onlyVal,onlyVal);
    return 0;
  }

  int *val = (int *) malloc(sizeof(int)*rows*cols);
  if (val == NULL) {
    printf("malloc error\n");
    return 1;
  }

  //building the matrix
  int tempVal;
  for(int i=0; i<rows*cols; i++){
    scanf(" %d", &tempVal);
    val[i] = tempVal;
    printf("%d ",tempVal);
    if(tempVal < 10){
      printf(" ");
    }
    if((i+1)%rows == 0){
      printf("\n");
    }
  }


  //getting the sum of the main diagonal
  int sum = 0;
  for(int i=0; i<rows*cols; i=i+cols+1){
    sum += val[i];
  }

  int thisSum = 0;


  //check backwards diagonal sum
  for(int i=cols-1; i<rows*(cols-1)+1; i=i+cols-1){
    thisSum += val[i];
  }
  if(sum == thisSum){
    thisSum = 0;
  } else {
    printf("this square is not magical :(\n");
    return 1;
  }

  //check row sums
  for(int i=0;i<rows;i++){
    for(int j=0; j<cols; j++){
      thisSum += val[i*cols+j];
    }
    if(thisSum == sum){
      thisSum = 0;
    } else {
      printf("this square is not magical :(\n");
      return 1;
    }
  }

  //check column sums
  for(int i=0; i<cols; i++){
    for(int j=0; j<rows; j++){
      thisSum += val[i+cols*j];
    }
    if(thisSum == sum){
      thisSum = 0;
    } else {
      printf("this square is not magical :(\n");
      return 1;
    }
  }

  printf("this matrix is magical!!! its constant is %d\n",sum);

  free(val);
  val = NULL;
  return 0;
}
