#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[]) {
  char* s = "Hello \0World";
  int i = 123;
  short t = 234;
  float f = 1234l;
  double d = 12345;
  int a[1000000];

  printf("String is: %s\n", s + 7);
  printf("int is: %d\n", i);
  printf("short is: %d\n", t);
  printf("float is: %.2f\n", f);
  printf("double is: %G\n", d);
  printf("hex is: %x\n", i);


  char ab = 255;
  char b = 2;
  char c = ab + b;
  printf("%d\n", c);

  int x = INT_MAX;
  x++; // become INT_MIN
  printf("%d\n", x);

  exit(2);
}
