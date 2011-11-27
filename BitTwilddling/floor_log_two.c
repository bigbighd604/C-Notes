#include <stdio.h>
#include <stdlib.h>

/*
 * http://en.wikipedia.org/wiki/Binary_logarithm
 */

/**
 * Returns the floor form of binary logarithm for a 32 bit integer.
 * −1 is returned if ''n'' is 0.
 */
int floorLog2(unsigned int n) {
  if (n == 0)
    return -1;
 
  int pos = 0;
  if (n >= (1 <<16)) { n >>= 16; pos += 16; }
  if (n >= (1 << 8)) { n >>=  8; pos +=  8; }
  if (n >= (1 << 4)) { n >>=  4; pos +=  4; }
  if (n >= (1 << 2)) { n >>=  2; pos +=  2; }
  if (n >= (1 << 1)) {           pos +=  1; }
  return pos;
}


int main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
  printf("floor_log_two(%d) = %d\n", n, floorLog2(n));
}
