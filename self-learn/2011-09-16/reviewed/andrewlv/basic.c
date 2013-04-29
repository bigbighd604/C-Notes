#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// leczb: General comment: clean, simple, elegant and does what's required.
//        Well done!

char *normalize_path(const char *folder_path, const char *entry_name) {
  char *both = malloc(strlen(folder_path) + strlen(entry_name) + 2);
  strcpy(both, folder_path);
  if (*(both + strlen(folder_path) - 1) != '/') {
    // leczb: Nice touch! ;)
    strcat(both, "/");
  }
  strcat(both, entry_name);
  return both;
}


void print_dir_entries(const char *pathname) {
  struct dirent **namelist;
  int number_of_names;

  number_of_names = scandir(pathname, &namelist, 0, alphasort);
  if (number_of_names < 0) {
    perror("scandir");
  }
  else {
    while (number_of_names--) {
      char *entry_name = namelist[number_of_names]->d_name;
      if (strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0) {
        continue;
      }
      printf("%s/%s\n", pathname, entry_name);
      if (namelist[number_of_names]->d_type == DT_DIR) {
        print_dir_entries(normalize_path(pathname, entry_name));
        // leczb: You have a memory leak here: normalize_path()
        //        malloc()s space for the new path, but you never
        //        free() it up.
      }
      free(namelist[number_of_names]);
    }
    free(namelist);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_dir_entries(".");
  } else {
    print_dir_entries(argv[1]);
  }
}
