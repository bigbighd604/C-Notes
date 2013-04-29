#include <stdio.h>
#include <stdint.h>

// Swap bytes of an integer(32bits)
// Position change: 1<->4, 2<->3

int main(int argc, char *argv[]) {
  uint32_t a = 869030054;
  //int a = 234;
  uint32_t b;
  uint32_t array[4];

  array[0] = a<<24;
  //array[1] = (a>>8)<<16 & 0xffffff;
  array[1] = (a<<8) & 0x00ff0000;
  //array[2] = (a>>16)<<8 & 0xffff;
  array[2] = (a>>8) & 0x0000ff00; //more efficient
  array[3] = a>>24 & 0x000000ff;

  printf("before swap, number is:%d, hex is: %X\n", a, a);
  b = array[0] | array[1] | array[2] | array[3];
  printf("after swap, number is:%d, hex is: %X\n", b, b);

}
