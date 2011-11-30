#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int simple_way(unsigned int v) {
  unsigned int c;

  for (c = 0; v; v >>=1) {
    c += v & 1;
  }
  return c;
}


static const unsigned char bits_table[256] = {
#define B2(n) n, n+1, n+1, n+2
#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
  B6(0), B6(1), B6(1), B6(2)
};


unsigned int lookup_table(unsigned int v) {
  unsigned int c;

  // Option 1
  c = bits_table[v & 0xff] + bits_table[(v >> 8) & 0xff] +
    bits_table[(v >> 16) & 0xff] + bits_table[v >> 24];

  // Option 2
  unsigned char *p = (unsigned char *) &v;
  c = bits_table[p[0]] + bits_table[p[1]] + bits_table[p[2]] +
    bits_table[p[3]];

  // Method 2 to generate table
  bits_table[0] = 0;
  for (i = 0; i < 256; i++) {
    bits_table[i] = (i & 1) + bits_table[i / 2];
  }

}


unsigned int brian_kernighan(unsigned int v) {
  unsigned int c;

  for (c = 0; v; c++) {
    v & = v - 1;
  }
  return c;
}


int main(int argc, char *argv[]) {
  unsigned int v;

}
