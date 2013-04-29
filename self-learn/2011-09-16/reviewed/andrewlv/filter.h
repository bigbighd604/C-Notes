#include <dirent.h>

// leczb: the #ifndef guard should be the very first line of the header file and
//        it should be named FILTER_H.
#ifndef FILTER
// leczb: I don't think it works. You need a "#define FILTER_H 1" line here.
typedef struct filters {
  int name_len;
  char *name;
  // leczb: These should have been defined as
  //        unsigned char d_type; // see man 3 readdir
  //        uid_t uid; // see man 2 stat
  //        gid_t gid;
  //        off_t size;
  unsigned int d_type;
  unsigned int uid;
  unsigned int gid;
  unsigned int size;
} FILTER;

#endif

extern void print_dir_entries(const char *pathname, FILTER *filter, int level);
// leczb: Minor naming comment: I would have named this "apply_filters".
extern int filter_filters(const char *pathname, const struct dirent *entry, const FILTER *filter);
extern void parse_opts_and_filter(int argc, char *argv[], FILTER *filter, int *pathindex, int *level);
