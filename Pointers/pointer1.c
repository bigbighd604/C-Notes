#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *p1 = "name";
  char *p2, *p3;

  p2 = (char *)malloc(20);
  memset(p2, 0, 20);
  p3 = p2;
  while (*p2++ = *p1++);
  printf("%s\n", p2); // p2 changed.
}
