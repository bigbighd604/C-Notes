#include <stdio.h>

int global = 2;
static int s;
const int a = 10;

void f(int a) {
  printf("%d\n", s);
  static int s = 0; //This will only be initialized once.

  s +=a;
  printf("%d\n", s);

}


int main(int argc, char *argv[]) {
  s = 10;
  f(2);
  f(4);
  printf("adfafd\n");

}
