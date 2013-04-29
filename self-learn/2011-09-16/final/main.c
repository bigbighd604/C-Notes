#include "filter.h"


int main(int argc, char *argv[]) {
  int pathindex = -1; //array index after parsing options
  int max_level = -1;
  FILTER filter;  //struct stores filter options

  //This function parse options and store them in to filter struct
  //And return array index to get directory names
  parse_opts_and_filter(argc, argv, &filter, &pathindex, &max_level);
  if (pathindex >= argc) {
    print_dir_entries(".", &filter, max_level);
  } else {
    for (pathindex; pathindex < argc; pathindex++) {
      print_dir_entries(argv[pathindex], &filter, max_level);
    }
  }
}
