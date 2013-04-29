#include <stdio.h>
#include <stdlib.h>

int strlength(char * data) {
  int counter = 0;
  while (*data++) {
    counter++;
  }
  return counter;
}


int main(int argc, char *argv[]) {
  char * txt = "this is a test";
  printf("%s\n", txt);
  int i;
  for (i = 0; i < argc; i++) {
    printf("Arg %d is: %s\t", i, argv[i]);
    printf("Arg length is: %d\n", strlength(argv[i]));
  }
}
