#include <stdio.h>

void f(int d) {
  int a[1000];
  printf("Level:%d; Addr:%lx\n", d, &d);
  f(d+1);
}


int main(int argc, char *argv[]) {
  f(1);
}
