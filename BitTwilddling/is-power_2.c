#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_power_of_2(int x) {
  return (x > 0) && !(x & (x - 1));
}

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
  if (is_power_of_2(n)) {
    printf("%d is a power of 2\n", n);
  } else {
    printf("%d is not a power of 2\n", n);
  }

}
