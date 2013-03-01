#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN ( 1024 )


/*
 * Reverse a C-style string('\0' at the end) in place.
 * i.e. abcdefg -> gfedcba
 */

void reverse_string(char *str) {
  char *end = str;
  char temp;

  if (str) {
    while (*end) {
      end++;
    }
    end--;

    while (str < end) {
      temp = *str;
      *str++ = *end;
      *end-- = temp;
    }
  }
}


int main(int argc, char *argv[]) {
  char *str;
  str = malloc(sizeof(char) * STRLEN);
  memset(str, STRLEN, sizeof(char));
  scanf("%s", str);
  reverse_string(str);
  printf("%s\n", str);
}
