#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

void encodeInBinary(struct ppm_pixel* image, const char* message, int size, int rows, int cols, char* filename);
void charToBinary(char c, unsigned char* binary);

int main(int argc, char** argv) {
  int rows;
  int cols;

  if(argc != 2){
    printf("no file found");
    return 1;
  }

  char* filename = argv[1];
  char* newfilename = malloc(sizeof(int)*strlen(filename)+1);
  strncpy(newfilename, filename, strlen(filename)-4);
  strcat(newfilename, "-encoded.ppm");

  struct ppm_pixel* image = read_ppm(filename,&rows,&cols);

  int numChars = (rows*cols*3)/8;
  printf("Reading %s with width %d and height %d\n", filename, rows, cols);
  printf("Max number of characters in the image: %d\n", numChars);

  // char* toEncode = malloc(sizeof(char)*numChars);
  // if(toEncode == NULL){
  //   printf("Malloc unsuccessful");
  //   return 1;
  // }
  char toEncode[100];
  printf("Enter a phrase: ");
  scanf("%s", toEncode);
  //fgets(toEncode, numChars, stdin);

  encodeInBinary(image, toEncode, numChars, rows, cols, newfilename);

  free(newfilename);
  newfilename = NULL;
  ///free(toEncode);
  //toEncode = NULL;
  free(image);
  image = NULL;
  return 0;
}

void charToBinary(char c, unsigned char* binary){
  for(int i=0; i<8; i++){
    //printf("%c",c);
    binary[i] = c >> (7-i) & 0x01;
    //printf("%c   %d\n",c, binary[i]);
  }
}

void encodeInBinary(struct ppm_pixel* image, const char* message, int size, int rows, int cols, char* filename){
  unsigned char* thisChar = malloc(sizeof(message+1)*8);
  memset(thisChar, 0, sizeof(message+1)*8);

  for(int i=0; i<size; i++){
    charToBinary(message[i], &thisChar[i*8]);
  }
  // for(int j=0; j<8*size; j++){
  //   printf("%d",thisChar[j]);
  // }
  // printf("\n");

  for(int i=0; i<rows*cols; i++){
    image[i].red = (image[i].red & 0xfe) | thisChar[i*3];
    image[i].green = (image[i].green & 0xfe) | thisChar[i*3+1];
    image[i].blue = (image[i].blue & 0xfe)  | thisChar[i*3+2];
  }

  printf("writing %s\n",filename);
  write_ppm(filename, image, rows, cols);

  free(thisChar);
  thisChar = NULL;
}
