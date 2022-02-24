#include <stdio.h>
#include <stdlib.h>

// readvector opens and reads the given file
// The size of the vector should be stored in the parameter size
// The values should be returned in a flat float array
float* readvector(const char* filename, int *size) {
  FILE* infile = NULL;
  infile = fopen(filename, "r");
  if (infile == NULL) {
      printf("Error open file\n");
      exit(1);
  }

  char buf[33];
  fgets(buf, 33, infile);
  *size = atoi(buf);

  float *vector = malloc(sizeof(float) * (*size));

  for(int i=0; i<(*size); i++){
    fgets(buf, 33, infile);
    vector[i] = (float) atof(buf);
  }
  fclose(infile);
  return vector;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: readvector <filename>\n");
    exit(0);
  }

  int size = 0;
  float* vector = readvector(argv[1], &size);

  // todo: print vector values
  for (int i = 0; i < size; i++) {
    printf("%f\n", vector[i]);
  }

  free(vector);
  vector = NULL;
  return 0;
}
