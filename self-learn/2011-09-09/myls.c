#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mylib.h"


// Implement 'ls -l' with sort
//

void simple(char *folder_path) {
  DIR *dir;
  struct dirent *entry;

  if ((dir = opendir(folder_path)) == NULL) {
    printf("Failed to open directory: %s\n", folder_path);
    exit(EXIT_FAILURE);
  }
  while (entry = readdir(dir)) {
    printf("%s\n", entry->d_name);
  }
}



void complexer(char *folder_path) {
  DIR *dir;
  int counter;
  int pointer = 0;
  int bulksize = 100;
  int buffersize = 200;
  struct dirent *entry;
  struct dirent **entries;
  struct stat file_stats;  //stores in stack, compiler reserves data in stack
  //struct stat *file_stats;  //compiler only reserves pointer space(4bytes) in stack, need to use malloc to assign space(in heap) before use it.
  if ((dir = opendir(folder_path)) == NULL) {
    printf("Failed to open directory: %s\n", folder_path);
    exit(EXIT_FAILURE);
  }

  entries = (struct dirent **)malloc(sizeof(struct dirent *) * buffersize);
  while (entry = readdir(dir)) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    if (pointer == buffersize - 1) {
      buffersize += bulksize;
      entries = (struct dirent **)realloc(entries, sizeof(struct dirent *) * buffersize);
    }
    entries[pointer] = entry;
    pointer++;
  }
  //while (entry = readdir(dir)) {
   // if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
    //  continue;
   // }

   sort(entries, pointer);
   for (counter = 0; counter < pointer; counter++) {
    entry = entries[counter];

    char *fixed_path = fix_path2(folder_path, entry->d_name);
    //if (lstat(fix_path2(folder_path, entry->d_name), &file_stats) == -1) {
    if (lstat(fixed_path, &file_stats) == -1) {
      perror("stat");
      exit(EXIT_FAILURE);
    }

    // handle file types
    switch (file_stats.st_mode & S_IFMT) {
      case S_IFBLK: printf("b"); break;
      case S_IFCHR: printf("c"); break;
      case S_IFDIR: printf("d"); break;
      case S_IFIFO: printf("p"); break;
      case S_IFLNK: printf("l"); break;
      case S_IFREG: printf("-"); break;
      case S_IFSOCK: printf("s"); break;
      default: printf("?"); break;
    }

    // file permissions
    (file_stats.st_mode & S_IRUSR)?printf("r"): printf("-");
    (file_stats.st_mode & S_IWUSR)?printf("w"): printf("-");

    // handle set uid bit
    if (file_stats.st_mode & S_IXUSR) {
      if (file_stats.st_mode & S_ISUID) {
        printf("s");
      } else {
        printf("x");
        }
    } else {
      if (file_stats.st_mode & S_ISUID) {
        printf("S");
      } else {
        printf("-");
        }
    }

    (file_stats.st_mode & S_IRGRP)?printf("r"): printf("-");
    (file_stats.st_mode & S_IWGRP)?printf("w"): printf("-");

    // handle set gid bit
    if (file_stats.st_mode & S_IXGRP) {
      if (file_stats.st_mode & S_ISGID) {
        printf("s");
      } else {
        printf("x");
        }
    } else {
      if (file_stats.st_mode & S_ISGID) {
        printf("S");
      } else {
        printf("-");
        }
    }
    (file_stats.st_mode & S_IROTH)?printf("r"): printf("-");
    (file_stats.st_mode & S_IWOTH)?printf("w"): printf("-");

    // handle stick bit
    if (file_stats.st_mode & S_IXOTH) {
      if (file_stats.st_mode & S_ISVTX) {
        printf("t");
      } else {
        printf("x");
        }
    } else {
      if (file_stats.st_mode & S_ISVTX) {
        printf("T");
      } else {
        printf("-");
        }
    }
    printf(" ");

    printf("%u ", (unsigned int)file_stats.st_nlink);
    printf("%s\t", get_username(file_stats.st_uid));
    printf("%s\t", get_groupname(file_stats.st_gid));
    printf("%u\t", (unsigned int)file_stats.st_size);
    printf("%s\t", format_time(file_stats.st_mtime));
    printf("%s\n", entry->d_name);
  }

}

int main(int argc, char *argv[]) {
  char *folder_path;

  if (argc > 1) {
    folder_path = argv[1];
    complexer(folder_path);
  }
}
