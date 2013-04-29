#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EVENT_SIZE ( sizeof(struct inotify_event) )
#define BUFFER_SIZE (100 * (EVENT_SIZE + 256))

void help(const char *progname) {
  printf("Usage: %s pathname1 pathname2 ...", progname);
  exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {
  int inotify_fd;
  int watch_fd;
  fd_set rfds;
  ssize_t bytes_returned;
  int select_return_value;
  char buffer[BUFFER_SIZE];

  char *pathname = argv[1];

  if (argc < 2) help(argv[0]);

  inotify_fd= inotify_init();
  if (inotify_fd == -1) {
    perror("inotify_init");
    exit(EXIT_FAILURE);
  }
  printf("inotify_fd == %d\n", inotify_fd);

  watch_fd = inotify_add_watch(inotify_fd, pathname, IN_ALL_EVENTS);
  watch_fd = inotify_add_watch(inotify_fd, "/tmp/test", IN_ALL_EVENTS);
  if (watch_fd == -1) {
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);
  }
  printf("watch_fd == %d\n", watch_fd);

  FD_ZERO(&rfds);
  FD_SET(inotify_fd, &rfds);

  while (1) {
    select_return_value = select(inotify_fd + 1, &rfds, NULL, NULL, NULL);
    if (select_return_value) {
      while (bytes_returned = read(inotify_fd, buffer, BUFFER_SIZE)) {
        int offset = 0;
        while (offset < bytes_returned) {
          struct inotify_event *event = (struct inotify_event *)&buffer[offset];
          printf("watch_fd == %d\n", event->wd);
          if (event->mask & IN_ACCESS) { printf("File Was Accessed:"); }
          if (event->mask & IN_ATTRIB) { printf("File Attribute Changed:"); }
          if (event->mask & IN_CLOSE_WRITE) {
            printf("File Was Closed with Write:");
          }
          if (event->mask & IN_CLOSE_NOWRITE) {
            printf("File Was Closed without Write:");
          }
          if (event->mask & IN_CREATE) { printf("File Was Created:"); }
          if (event->mask & IN_DELETE) { printf("File Was Deleted:"); }
          if (event->mask & IN_DELETE_SELF) {
            printf("Monitored File Was Deleted:");
          }
          if (event->mask & IN_MODIFY) { printf("File Was Modified:"); }
          if (event->mask & IN_MOVE_SELF) {
            printf("Monitored File Was Moved Out:");
          }
          if (event->mask & IN_MOVED_FROM) { printf("File Was Moved Out:"); }
          if (event->mask & IN_MOVED_TO) { printf("File Was Moved In:"); }
          if (event->mask & IN_OPEN) { printf("File Was Opened:"); }
          if (event->mask & IN_CREATE) { printf("File Was Created:"); }

          if (event->len > 0) {
            printf("%s\n", event->name);
            offset = offset + EVENT_SIZE + event->len;
          } else {
            printf("\n");
            offset = offset + EVENT_SIZE;
          }
        }
      }
      if (bytes_returned == -1) {
        perror("read()");
        exit(EXIT_FAILURE);
      }
    } else {
      perror("select()");
    }
  }
  inotify_rm_watch(inotify_fd, watch_fd);
}
