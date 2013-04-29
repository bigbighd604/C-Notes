#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sys_call(char * file_path) {
  int fd;
  char line[1024];
  int count = 1024;
  ssize_t nread;
  if ((fd = open(file_path,O_RDONLY)) == -1) {
    printf("Failed to open file: %s", file_path); 
    exit(EXIT_FAILURE);
  }
  while (nread = read(fd, line, count)) {
    write(1, line, nread);
  }
}

void library_call(char * file_path) {
  FILE *fp;
  char line[1024];
  int count = 1024;
  size_t nread;
  if ((fp = fopen(file_path, "r")) == NULL) {
    printf("Failed to oepn file: %s", file_path);
    exit(EXIT_FAILURE);
  } 

  while (nread = fread(line, 1, count, fp)) {
    fwrite(line, 1, nread, stdout);
  }
}


void sys_copy_stdin() {
  char buffer[2048];
  int nread;
  while (nread = read(0, buffer, 2048)) {
    write(1, buffer, nread);
  }
}


void lib_copy_stdin() {
  char buffer[2048];
  int count = 2048;
  int nread;

  while (nread = fread(buffer, 4, 2048, stdin)) {
    fwrite(buffer, 4, nread, stdout);
  }
}


int main(int argc, char *argv[]) {
  if (argc > 1) {
    sys_call(argv[1]);
    library_call(argv[1]);
  } else {
    //sys_copy_stdin();
    lib_copy_stdin();
  }
}
