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
      apply_filters(pathname, namelist[number_of_names], filter);
      if (namelist[number_of_names]->d_type == DT_DIR) {
        /* fix memory leak according to leczb's suggestion. */
        char *sub_folder_path = normalize_path(pathname, entry_name);
        print_dir_entries(sub_folder_path, filter, level);
        free(sub_folder_path);
      }
      free(namelist[number_of_names]);
    }
    free(namelist);
  }
}


//function to filter out files based on struct value of filter
int apply_filters(const char *pathname, const struct dirent *entry,
                  const FILTER *filter) {
  if (filter->name_len > 0) {
    if (strcmp(entry->d_name, filter->name) != 0) return 0;
  }
  if (filter->d_type != DT_UNKNOWN) {
    if (entry->d_type != filter->d_type) return 0;
  }
  if (filter->uid != -1 || filter->gid != -1 || filter->size != -1) {
    struct stat file_stat;
    if (lstat(normalize_path(pathname, entry->d_name), &file_stat) == -1) {
      perror("lstat");
      return 0; /* return 0 based on suggestion of leczb. */
//      exit(EXIT_FAILURE);
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
  filter->d_type = DT_UNKNOWN;
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
        /* add "break;" to bail out and skip rest statements. */
        if (strlen(optarg) != 1) { print_help(argv[0]); break; }
        if (strcmp(optarg, "l") == 0) { filter->d_type = DT_LNK; break; }
        if (strcmp(optarg, "d") == 0) { filter->d_type = DT_DIR; break; }
        if (strcmp(optarg, "f") == 0) { filter->d_type = DT_REG; break; }
        if (strcmp(optarg, "c") == 0) { filter->d_type = DT_CHR; break; }
        if (strcmp(optarg, "b") == 0) { filter->d_type = DT_BLK; break; }
        if (strcmp(optarg, "s") == 0) { filter->d_type = DT_SOCK; break; }
        print_help(argv[0]);
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
