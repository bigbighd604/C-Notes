#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
  const char *p1;
  char const *p2;

  p1 = strdup("aaaaa");
  p2 = strdup("bbbbb");

  printf("p1=%s\n", p1);
  printf("p2=%s\n", p2);

  *p1 = 'b'; // not allowed to modify.
  printf("p1=%s\n", p1);

  p2 = "abcde";
  printf("p2=%s\n", p2);

}
