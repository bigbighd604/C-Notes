#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {

  int n = atoi(argv[1]);
  n = n - 1;
  n = n | (n >> 1);
  n = n | (n >> 2);
  n = n | (n >> 4);
  n = n | (n >> 8);
  n = n | (n >> 16);
  n = n + 1;
  printf("Rounded up to power of two: %d\n", n);

}
