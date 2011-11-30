#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {

  int n = atoi(argv[1]);
  int k;

  if ( n == 0) printf("Nearest power of 2: 1\n");

  for (k = sizeof(int) * 8 - 1; ((1 << k) & n) == 0; k--);

  if (((1 << (k - 1)) & n ) == 0) {
    printf("Nearest power of 2: %d\n", 1 << k);
  } else {
    printf("Nearest power of 2: %d\n", 1 << (k + 1));
  }

}
