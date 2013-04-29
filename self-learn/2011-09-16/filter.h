#include <dirent.h>

#ifndef FILTER
typedef struct filters {
  int name_len;
  char *name;
  unsigned int d_type;
  unsigned int uid;
  unsigned int gid;
  unsigned int size;
} FILTER;

#endif

extern void print_dir_entries(const char *pathname, FILTER *filter, int level);
extern int filter_filters(const char *pathname, const struct dirent *entry, const FILTER *filter);
extern void parse_opts_and_filter(int argc, char *argv[], FILTER *filter, int *pathindex, int *level);
