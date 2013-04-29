#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void print_help(char *name) {
  fprintf(stderr, "Usage: %s [-f arguments]\n", name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int i;
  int opt;
  int s_flag = 0;
  int f_option = 0;
  char *arg;
  if (argc < 2) {
    print_help(argv[0]);
  }

  while ((opt = getopt(argc, argv, "f:s")) != -1) {
    switch(opt) {
      case 'f':
        f_option = 1;
        arg = optarg;
        break;
      case 's':
        s_flag = 1;
        break;
      default:
        print_help(argv[0]);
    }
  }
  if (f_option) {
    printf("Flag '-f' is passed, and arg is: %s\n", arg);
  } else {
    printf("Flag '-f' is not passed\n");
  }
  if (s_flag) {
    printf("Flag '-s' is passed\n");
  } else {
    printf("Flag '-s' is not passed\n");
  }
  if (optind >= argc) {
    fprintf(stderr, "Expected argument after options\n"); //like filenames etc.
    exit(EXIT_FAILURE);
  }
  while (optind < argc) {
    printf("name argument = %s\n", argv[optind]);
    optind++;
  }
}
