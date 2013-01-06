#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int i;
  for (i=0; i < 2; i++) {
    fork();
    // How many "-" will be outputed? And why?
    printf("-");
    // How many lines of the following printf will generate?
    // Is it the same number of lines when output to stdout and file?
    // printf("-, pid=%d; \n", getpid());
  }

  // If want to see fork tree, enable the following line
  // and run: pstree -p | grep a.out
  // sleep(10);

  return 0;
}
