#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


char *fix_path(const char *folder_path, const char *file_name) {
  int i = 0;
  char *pointer;
  char *fixed_path;
  int folder_length = strlen(folder_path);
  int file_length = strlen(file_name);
  fixed_path = (char *)malloc(sizeof(char) * (folder_length + file_length + 1));
  pointer = fixed_path;

  for (i = 0; i< folder_length; i++) {
    *(pointer++) = *(folder_path++);
  }
  *(pointer++) = '/';
  for (i = 0; i< file_length; i++) {
    *(pointer++) = *(file_name++);
  }
  printf("%s\n", fixed_path);
  return fixed_path;
}


char *fix_path2(const char *folder_path, const char *file_name) {
  char *both = malloc(strlen(folder_path) + strlen(file_name) + 2);
  strcpy(both, folder_path);
  if (*(both + strlen(folder_path) - 1) != '/') {
    strcat(both, "/");  
  }
  strcat(both, file_name);
  return both;
}


char *get_username(const uid_t st_uid) {
  struct passwd *entry;

  entry = getpwuid(st_uid);
  if (entry == NULL) {
    return "Un Known";
  }
  return entry->pw_name;
}


char *get_groupname(const gid_t st_gid) {
  struct group *entry;

  entry = getgrgid(st_gid);
  if (entry == NULL) {
    return "Un Known";
  }
  return entry->gr_name;
}


char *format_time(const time_t st_time) {
  char *buf;
  int bufsize = 20;
  struct tm *ts;

  buf = malloc(sizeof(char) * bufsize);
  ts = localtime(&st_time);
  strftime(buf, bufsize, "%Y-%m-%d %H:%M:%S", ts);
  return buf;
}

int cmpstr(const void *p1, const void *p2) {
  //p1 and p2 are all pointers to array(1 layer, or 2 layers)
  struct dirent * const *name_one = p1;
  struct dirent * const *name_two = p2;
  return strcmp((*name_one)->d_name, (*name_two)->d_name);
}


void sort(void **entries, int number) {
  //qsort(entries, size, sizeof(struct dirent *), cmpstr);
  qsort(entries, number, sizeof(*entries), cmpstr); //sizeof element(pointer)
}
