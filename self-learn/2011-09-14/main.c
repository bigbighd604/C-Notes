#include <stdio.h>
#include "filter.h"


int select_even(const void *integer) {
  if ((*(int *)integer) % 2) {
    return 0;
  }
  return 1;
}


int main(int argc, char *argv[]) {
  int i;
  int input_array[10] = {1, 3, 7, 6, 9, 13, 15, 20, 37, 40};
  size_t output_number;
  //int output_array[10];
  int *output_array = NULL;

  filter((void *)input_array,
         sizeof(int),
         10,
         (void **)&output_array,
         (void *)&output_number,
         select_even);
  printf("%d even numbers:\n", (int)output_number);
  for (i = 0; i < output_number; i++) {
    printf("%d ", output_array[i]);
  }
  printf("\n");
}
