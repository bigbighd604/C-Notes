#ifndef _FILTER_H
#define _FILTER_H   1

#include <dirent.h>
#include <sys/types.h>

typedef struct filters {
  int name_len;
  char *name;
  unsigned char d_type; /* use correct declaration type */
  uid_t uid;
  gid_t gid;
  off_t size;
} FILTER;


extern void print_dir_entries(const char *pathname, FILTER *filter, int level);
extern int apply_filters(const char *pathname, const struct dirent *entry, const FILTER *filter);
extern void parse_opts_and_filter(int argc, char *argv[], FILTER *filter, int *pathindex, int *level);


#endif
