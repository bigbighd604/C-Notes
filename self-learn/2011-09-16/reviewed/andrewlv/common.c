#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filter.h"

void print_help(char *progname) {
  // leczb: Minor: it should be const char *
  char *opts = "[-n name] [-t l|d|f|o|b|s] [-o uid] [-g gid] [-s size(bytes]] [-d level]";
  fprintf(stderr, "Usage: %s %s\n", progname, opts);
  exit(EXIT_FAILURE);
}


// concatenate folder path and entry name under the folder path
char *normalize_path(const char *folder_path, const char *entry_name) {
  char *both = malloc(strlen(folder_path) + strlen(entry_name) + 2);
  strcpy(both, folder_path);
  // leczb: Minor style comment: array subscript notation is preferred in this
  //        case:
  //          if (both[strlen(folder_path) - 1] != '/') {
  if (*(both + strlen(folder_path) - 1) != '/') {
    strcat(both, "/");
  }
  // leczb: ...and here as well.
  if (*(entry_name + strlen(entry_name) - 1) == '/') {
    strncat(both, entry_name, strlen(entry_name) - 1);
  } else {
    strcat(both, entry_name);
  }
  return both;
}
