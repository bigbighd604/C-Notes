#include <stdio.h>

int main(int argc, char *argv[]) {
  char * const p1 = "This is a const pointer, point to char(string literal)";
  //const char *p1 = "Text" === char *p1 = "Text 2", string literal
  const char * p2 = "This is a pointer, point to const char";
  const char * const p3 = "This is a const pointer, point to const char";
  char * p4 = "This is a normal pointer";

  //p1 = p3; //Not allowed when compiling.
  //p1[1] = 'X';
  p4[0] = 'S';
  p2 = p3;
  //p3 = p4; //Not allowed when compiling.
  //p3[0] = 'S'; //Not allowed when compiling.
  printf("p1: %s\n", p1);
  printf("p2: %s\n", p2);
  printf("p3: %s\n", p3);
  printf("p4: %s\n", p4);
}
