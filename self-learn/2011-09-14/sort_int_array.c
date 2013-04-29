#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 20

int int_array[ARRAY_SIZE] = {1,2};
char *txt = "aaaaaaaaaaaaaa";
const int int_number_initialized = 1;
const int int_number_not_initialized;


int int_cmp(const void *int1, const void *int2) {
  if (*(int *)int1 == *(int *)int2) {
    return 0;
  }
  return (*(int *)int1 > *(int *)int2)? 1:-1;
}


int int_cmp_desc(const void *int1, const void *int2) {
  if (*(int *)int1 == *(int *)int2) {
    return 0;
  }
  return (*(int *)int1 < *(int *)int2)? 1:-1;
}


void generate_array(int array[], int size) {
  int counter;
  for (counter = 0; counter < size; counter++) {
    array[counter] = random() % 1000;
  }
}


void print_array(int array[], int size) {
  int counter;
  for (counter = 0; counter < size; counter++) {
    printf("%d ", array[counter]);
  }
  printf("\n");
}


int main(int argc, char *argv[]) {
  generate_array(int_array, ARRAY_SIZE);
  printf("Before sorting:\nArray is:");
  print_array(int_array, ARRAY_SIZE);
  qsort(int_array, ARRAY_SIZE, sizeof(int), int_cmp_desc);
  printf("After sorting:\nArray is:");
  print_array(int_array, ARRAY_SIZE);
}
