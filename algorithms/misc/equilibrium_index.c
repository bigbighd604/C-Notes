#include <stdio.h>

void equilibrium_index(int array[], int size) {
  int i, sum, lsum;
  sum = 0;
  lsum = 0;
  for (i = 0; i < size; i++) {
    sum += array[i];
  }

  for (i = 0; i < size; i ++) {
    sum -= array[i];
    if (sum == lsum) {
      printf("index: %d\n", i);
    }
    lsum += array[i];
  }

}

int main(int argc, char *argv[]) {
  int list[] = {-7, 1, 5, 2, -4, 3, 0};
  int size = sizeof(list) / sizeof(int);
  equilibrium_index(list, size);

}
