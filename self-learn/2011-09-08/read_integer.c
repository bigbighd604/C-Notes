#include <stdio.h>
#include <stdlib.h>


// read integers from standard input
// print them out when press Ctrl-D(EOF)
// print the average number at the same time


int main(int argc, char *argv[]) {
  int i;
  int sum = 0;
  int number;
  int pointer = 0;
  int * numbers;
  size_t size = 2;
  printf("%G\n", sizeof(double));

  if ((numbers = (int *)malloc(sizeof(int) * size)) == NULL) {
    exit(EXIT_FAILURE);
  }

  while (scanf("%i", &number) != EOF) {
    numbers[pointer++] = number;
    //if (pointer > size / 2) {
    if (pointer == size - 1) {
      numbers = (int *)realloc(numbers, sizeof(int) * (size *= 2));
    }
  }

  printf("All valid numbers are:\n");
  for (i = 0; i < pointer; i++) {
    printf("%d, ", numbers[i]);
    sum += numbers[i];
  }
  printf("\nAverage number is: %d\n", sum / pointer);
  free(numbers);
}
