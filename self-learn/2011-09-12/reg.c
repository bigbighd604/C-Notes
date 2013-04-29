#include <sys/types.h>
#include <libgen.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int ret;
  regex_t preg;
  printf("REG_NEWLINE: %d\n", REG_NEWLINE);
  printf("REG_EXTENDED: %d\n", REG_EXTENDED);
  printf("REG_ICASE: %d\n", REG_ICASE);
  printf("REG_NOSUB: %d\n", REG_NOSUB);
  if (regcomp(&preg, "print", 0) == 0) {
    //perror("regcomp");
    printf("success!\n");
    //printf("%s\n", preg);
  }
}
