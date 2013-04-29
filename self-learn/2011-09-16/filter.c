#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "filter.h"


//recursive functions to list filenames and apply filters.
void print_dir_entries(const char *pathname, FILTER *filter, int level) {
  struct dirent **namelist;
  int number_of_names;

  if (level == 0) return;
  if (level > 0) level--;
  number_of_names = scandir(pathname, &namelist, 0, alphasort);
  if (number_of_names < 0) {
    fprintf(stderr, "scandir process %s error:", pathname);
    perror("");
  }
  else {
    while (number_of_names--) {
      char *entry_name = namelist[number_of_names]->d_name;
      if (strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0) {
        continue;
      }
      filter_files(pathname, namelist[number_of_names], filter);
      if (namelist[number_of_names]->d_type == DT_DIR) {
        print_dir_entries(normalize_path(pathname, entry_name), filter, level);
      }
      free(namelist[number_of_names]);
    }
    free(namelist);
  }
}


//function to filter out files based on struct value of filter
int filter_files(const char *pathname,
           const struct dirent *entry,
           const FILTER *filter) {
  if (filter->name_len > 0) {
    if (strcmp(entry->d_name, filter->name) != 0) return 0;
  }
  if (filter->d_type != -1) {
    if (entry->d_type != filter->d_type) return 0;
  }
  if (filter->uid != -1 || filter->gid != -1 || filter->size != -1) {
    struct stat file_stat;
    if (lstat(normalize_path(pathname, entry->d_name), &file_stat) == -1) {
      perror("lstat");
      exit(EXIT_FAILURE);
    }
    if ((filter->uid != -1) && (file_stat.st_uid != filter->uid)) return 0;
    if ((filter->gid != -1) && (file_stat.st_gid != filter->gid)) return 0;
    if ((filter->size != -1) && (file_stat.st_size < filter->size)) return 0;
  }
  printf("%s/%s\n", pathname, entry->d_name);
}



void parse_opts_and_filter(int argc, char *argv[], FILTER *filter,
                           int *pathindex, int *level) {
  int opt;
  int name_len;
  filter->name_len = 0; //initialize struct with default values
  filter->d_type = -1;
  filter->uid = -1;
  filter->gid = -1;
  filter->size = -1;
 while ((opt = getopt(argc, argv, "n:t:o:g:s:d:")) != -1) {
    switch(opt) {
      case 'n':
        name_len = strlen(optarg);
        if (name_len > 0) {
          filter->name_len = name_len;
          filter->name = malloc(name_len + 1);
          strcpy(filter->name, optarg);
        } else {
          print_help(argv[0]);
        }
        break;
      case 't':
        if (strlen(optarg) != 1) print_help(argv[0]);
        if (strcmp(optarg, "l") == 0) filter->d_type = DT_LNK;
        if (strcmp(optarg, "d") == 0) filter->d_type = DT_DIR;
        if (strcmp(optarg, "f") == 0) filter->d_type = DT_REG;
        if (strcmp(optarg, "c") == 0) filter->d_type = DT_CHR;
        if (strcmp(optarg, "b") == 0) filter->d_type = DT_BLK;
        if (strcmp(optarg, "s") == 0) filter->d_type = DT_SOCK;
        break;
      case 'o':
        filter->uid = atoi(optarg);
        break;
      case 'g':
        filter->gid = atoi(optarg);
        break;
      case 's':
        filter->size = atoi(optarg);
        break;
      case 'd':
        if (atoi(optarg) <= 0) print_help(argv[0]);
        *level = atoi(optarg);
        break;
      default:
        print_help(argv[0]);
    }
  }
 *pathindex = optind;
}
