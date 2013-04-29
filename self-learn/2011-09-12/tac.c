#include <stdio.h>
#include <stdlib.h>

void my_tac(FILE *fp) {
  size_t bufsize = 2048;
  char buffer[bufsize];
  int amend_offset = 0;
  int pointer_offset = 0;
  int number_of_reads = 0;
  int effective_pointer_offset = 0;
  int available_bytes_size = 0;

  while (1) {
    pointer_offset += bufsize - amend_offsite;
    effective_pointer_offset = fseek(fp, pointer_offset, SEEK_END);
    if (effective_pointer_offset == -1) {
      perror("fseek");
      exit(EXIT_FAILURE);
    }
    if (effective_pointer_offset < pointer_offset) {
      available_bytes_size = bufsize - (pointer_offset - available_bytes_size);
    } else {
      available_bytes_size = bufsize;
    }
    number_of_reads = fread(&buffer, sizeof(buffer), available_bytes_size, fp);

  }

}


int main(int argc, char *argv[]) {
  if (argc > 1) {
    int counter;
    FILE *fp;
    for (counter = 1; counter < argc, counter++) {
      fp = fopen(argv[counter], "r");
      if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
      }
      my_tac(fp);
    }
  } else {
    printf("Usage: %s filename1 filename2 ...\n", basename(argv[0]));
  }
}
