#include <stdio.h>
#include <stdlib.h>

int is_power_of_two_while(unsigned int n) {
  int i = 1;

  if (n == 1) return 1;

  while (i < n) {
    i *= 2;
  }

  if (i == n) return 1;

  return 0;
}


int is_power_of_two(unsigned int n) {
  unsigned int i;
  unsigned int counter = 0;
  unsigned int int_length = sizeof(int) * 8;

  for (i = int_length; i > 0; i--) {
    counter += n & 1;
    if (counter > 1) break;
    n >>= 1;
  }

  return (counter == 1);
}


int is_power_of_two_faster(unsigned int n) {
  unsigned int i;
  unsigned int int_length = sizeof(int) * 8;

  if (n == 0) return 0;

  for (i = int_length; i> 0; i--) {
    if (n & 1) break;
    n >>= 1;
  }

  return (n >> 1 == 0); //# After got 1, remain should be zero.
}


int pow2(unsigned int n) {
  return n != 0 &&  (n & (n - 1-) == 0);
}

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
  if (is_power_of_two_while(n)) {
    printf("%d is a power of 2\n", n);
  } else {
    printf("%d is not a power of 2\n", n);
  }
}
