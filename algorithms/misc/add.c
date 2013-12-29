#include <stdio.h>

/*
 * Write a function to add 2 numbers, not use + or any arithmetic operators.
 */

int add(int a, int b) {
  if (b == 0) return a;
  int sum = a ^ b;
  int carry = (a & b) << 1;
  return add(sum, carry);
}

int main(int argc, char* argv[]) {
  char* str_a = argv[1];
  char* str_b = argv[2];
  int sum = add(atoi(str_a), atoi(str_b));
  printf("Sum: %d\n", sum);
}
