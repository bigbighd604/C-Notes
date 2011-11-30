#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct {
  signed int x:5;
} s;

int main(int argc, char *argv[]) {
  int x = 20;
  int r;

  // x = 00010100 (20)
  // s.x = 10100  (-12)
  // r = 11110100 (-12)
  r = s.x = x;
  printf("r=%d, s.x=%d, x=%d\n", r, s.x, x);

}
