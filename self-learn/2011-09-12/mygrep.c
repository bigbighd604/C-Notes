#include <sys/types.h>
#include <libgen.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// mygrep <search string> filename filename ...

void regex_grep(FILE *fp, const char *regex_string) {
  char *line;
  int line_number = 1;
  size_t length = 0;
  ssize_t nread;
  regex_t preg;
  //size_t nmatch = 1;
  //regmatch_t pmatch[1];
  printf("I am here\n");
  if (regcomp(&preg, regex_string, 0) != 0) {
    perror("regcomp");
    exit(EXIT_FAILURE);
  }
  printf("I am not here\n");
  while ((nread = getline(&line, &length, fp)) != -1) {
    if (regexec(&preg, line, 0, NULL, 0) == 0) {
      printf("[Line:%d]%s", line_number, line);
    }
    line_number++;
  }
  if (line) {
    free(line);
  }
  if (&preg) {
    regfree(&preg);
  }
  fclose(fp);
}

void mygrep(FILE *fp, const char *search_string) {
  char *line;
  int line_number = 1;
  size_t length = 0;
  ssize_t nread;

  while ((nread = getline(&line, &length, fp)) != -1) {
    if ( strstr(line, search_string) != NULL) {
      printf("[Line:%d]%s", line_number, line);
    }
    line_number++;
  }
  if (line) {
    free(line);
  }
  fclose(fp);
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    int counter;
    FILE *fp;
    char *search_string;

    search_string = argv[1];
    for (counter = 2; counter < argc; counter++) {
      fp = fopen(argv[counter], "r");
      if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
      }
      printf("Greping file: %s\n", argv[counter]);
      //mygrep(fp, search_string);
      regex_grep(fp, search_string);
    }
  } else {
    printf("usage: %s keyword filename1 filename2 ...\n", basename(argv[0]));
  }
}
