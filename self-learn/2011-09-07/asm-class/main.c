
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// always keep the same number of elements in both arrays
int g_numbers1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int g_numbers2[] = {1, 2, 1, 3, 5, 3, 4, 3, 5,  2};
int g_numbers1_count;
int g_numbers2_count;
int g_input1;
int g_input2;

// reserve space for 1000 numbers
int g_output[1000];
int g_output_count;
int g_fibonacci_number;
int g_factorial_number;
int g_gcd_number1;
int g_gcd_number2;
int g_total;


static void sum_numbers(void) {
  int s = 0;
  int i;
  for (i = 0; i < g_numbers1_count ; i++) {
    s = s + g_numbers1[i];
  }
  g_total = s;
}

static void print_total(char* s, int x) {
  printf("%s\ng_total should be %d, it is: %d\n\n", s, x, g_total);
}

static void print_output(char* s) {
  int i;
  printf("%s\ng_output is:", s);
  for (i = 0; i < g_numbers1_count; i++) {
    printf(" %d ", g_output[i]);
  }
  printf("\n\n");
}

int main(int argc, char**argv) {

  g_numbers1_count = sizeof(g_numbers1) / sizeof(int);
  g_numbers2_count = sizeof(g_numbers2) / sizeof(int);

//  if (g_numbers1_count != g_numbers2_count) {
 //   printf("Number of elements should be the same in both arrays.");
  ////  abort();
  //}

  asm_example1();
  print_total("example1", 512);

  asm_example2();
  print_output("example2");

  g_input1 = 10;
  asm_sum_counter();
  print_total("sum_counter", 55);

  //sum_numbers();
  asm_sum_numbers();
  print_total("sum_numbers", 55);

  asm_add_sub_numbers();
  print_total("add_sub_numbers", 23);

  //sub_array_numbers();
  asm_array_numbers();
  print_output("array_numbers");

  g_fibonacci_number = 100;
  asm_fibonacci_number();
  int i;
  printf("g_fibonacci_numbers are:\n");
  for (i = 0; i < g_fibonacci_number; i++) {
    printf(" %d ", g_output[i]);
  }
  printf("\n\n");

  g_factorial_number = 10;
  asm_factorial_number();
  print_total("asm_factorial_number:", 3628800);

  g_gcd_number1 = 867;
  g_gcd_number2 = 1989;
  asm_gcd_number();
  print_total("asm_gcd_number:", 51);

  return 0;
}
