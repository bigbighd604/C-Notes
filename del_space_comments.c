#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define LINE_MAX  ( 2048 )

/*
 * Get from sudo source code: common/fileops.c
 */

int main(int argc, char *argv[]) {
  size_t len;
  char *cp = NULL;
  static char buf[LINE_MAX];

  if (fgets(buf, sizeof(buf), stdin) != NULL) {
    // Remove comments
    if ((cp = strchr(buf, '#')) != NULL) *cp = '\0';
  }

  // Remove leading and trailing space/newline.
  len = strlen(buf);
  while (len > 0 && isspace((unsigned char)buf[len - 1])) {
    buf[--len] = '\0';
  }

  for (cp = buf; isblank((unsigned char)*cp); cp++) continue;

  printf("%s\n", cp);
}
