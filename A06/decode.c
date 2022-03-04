#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"

int getMessage(struct ppm_pixel* image, int rows, int cols, char* message);
void decode(char* encoded, char* empty, int len);

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
  message = malloc((sizeof(char)*rows*cols*3)+8);
  if (message == NULL){
    printf("malloc failed");
    return 1;
  }
  int size;
  size = getMessage(image, rows, cols, message);

  char* decoded;
  decoded = malloc((sizeof(char)*size));
  if (decoded == NULL){
    printf("malloc failed");
    return 1;
  }
  decode(message, decoded, size);

  printf("%s\n", decoded);

  free(image);
  free(message);
  free(decoded);
  image = NULL;
  message = NULL;
  decoded = NULL;
  return 0;
}

int getMessage(struct ppm_pixel* image, int rows, int cols, char* message){
  //int i = 0;
  //while(val.red != '\0'){
  for(int i=0; i<rows*cols; i++){
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
  return size;
}

void decode(char* encoded, char* empty, int len){
  int numChars = len/8;
  for(int i=0; i<numChars; i++){
    char thisChar[9];
    for(int j=0; j<8; j++){
      thisChar[j] = encoded[(i*8)+j];
    }
    empty[i] = strtol(thisChar, 0, 2);
  }
}
