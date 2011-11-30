#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int parse_int(const char *str, int base) {
  char *endptr;
  long val;

  errno = 0;    /* To distinguish success/failure after call */
  val = strtol(str, &endptr, base);

  /* Check for various possible errors */

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
          || (errno != 0 && val == 0)) {
      perror("strtol");
      exit(EXIT_FAILURE);
  }

  if (endptr == str) {
      fprintf(stderr, "No digits were found\n");
      exit(EXIT_FAILURE);
  }

  /* If we got here, strtol() successfully parsed a number */

  printf("strtol() returned %ld, %lx\n", val, val);

  if (*endptr != '\0')        /* Not necessarily an error... */
      printf("Further characters after number: %s\n", endptr);
  return val;
}

int main(int argc, char *argv[]) {
  int v = parse_int(argv[1], 10);
  unsigned int r;

  printf("Origin = %d(dec), %x(hex)\n", v, v);

  // if v > 0, then mask == 0, if v < 0, then mask == -1
  int const mask = v >> (sizeof(int) * CHAR_BIT - 1);
  printf("Mask = %d(dec), %x(hex)\n", mask, mask);

  r = (v + mask) ^ mask;

  // Patented version:
  // r = (v ^ mask) - mask
  printf("Abs = %d\n", r);
}
