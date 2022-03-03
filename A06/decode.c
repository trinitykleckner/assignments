#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"

char* decode(char* encoded, int len);

int main(int argc, char** argv) {
  int rows;
  int cols;

  if(argc != 2){
    printf("no file found");
    return 1;
  }

  char* filename = argv[1];
  struct ppm_pixel* image = read_ppm(filename,&rows,&cols);
  printf("Reading %s with width %d and height %d\n", filename, rows, cols);
  printf("Max number of characters in the image: %d\n", (rows*cols*3)/8);
  char* message;
  message = malloc(sizeof(char)*rows*cols*3);
  if (message == NULL){
    printf("malloc failed");
    return 1;
  }
  //int i = 0;

  //while(val.red != '\0'){
  for(int i=0; i<rows*cols; i++){
    printf("Red: %d\nGreen: %d\nBlue: %d\n",image[i].red,image[i].blue,image[i].green);

    if(image[i].red%2 == 0){
      message[i*3] = '0';
    } else {
      message[i*3] = '1';
    }
    if(image[i].green%2 == 0){
      message[i*3+1] = '0';
    } else {
      message[i*3+1] = '1';
    }
    if(image[i].blue%2 == 0){
      message[i*3+2] = '0';
    } else {
      message[i*3+2] = '1';
    }
    //i++;

  }

  int size = rows*cols*3;
  while(size%8 != 0){
    message[size] = '0';
    size++;
  }
  message[size] = '\0';

  printf("%s\n",message);
  return 0;
}

/*
char* decode(char* encoded, int len){
  char* decoded[len];
  int counter = 0;
  for(int i=0; i<len/8; i++){
    char* thisChar[9];
    for(int j=0; j<8; j++){
      strcat(*thisChar, encoded[(counter*8)+1]);
    }
    char c = strtol(thisChar, 0, 2);
    strcat(*decoded,c);
    counter++;
} */
