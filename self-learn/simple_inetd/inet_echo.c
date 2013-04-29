#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>


#define BUFF_SIZE (1 << 8)

/*
 *  Simplified echo server using poll.
 *  It will be invoked by inetd.
 *
 */

void poll_events() {
  int nfds = 1;
  int length = 0;
  struct pollfd *fds;
  char buffer[BUFF_SIZE];
  ssize_t read_len, write_len;
  ssize_t total_read_len, total_write_len;

  fds = calloc(nfds, sizeof(struct pollfd));
  fds[0].fd = 0;
  fds[0].events = POLLIN;
  read_len = 0;
  write_len = 0;
  total_read_len = 0;
  total_write_len = 0;

  while (1) {
    if (poll(fds, nfds, -1) < 0) {
      syslog(LOG_ERR, "poll: %s", strerror(errno));
      continue;
    }
    if ((fds[0].revents & POLLHUP) || (fds[0].revents & POLLERR)) {
      syslog(LOG_INFO, "Connection closed! Total read:%lu, write:%lu\n",
             total_read_len, total_write_len);
      exit(EXIT_SUCCESS);
    }
    if (fds[0].revents & POLLIN) {
      int available_length = BUFF_SIZE - length;
      read_len = read(fds[0].fd, buffer+length, available_length);
      syslog(LOG_INFO, "Read length: %lu\n", read_len);
      if (read_len == 0) {
        if (length >0) {
          fds[0].events = POLLOUT;
        } else {
          syslog(LOG_INFO, "Connection closed! Total read:%lu, write:%lu\n",
                 total_read_len, total_write_len);
          exit(EXIT_SUCCESS);
        }
      } else if (read_len < 0) {
        syslog(LOG_ERR, "read: %s", strerror(errno));
      } else {
        length += read_len;
        total_read_len += read_len;
        if (length == BUFF_SIZE) {
          fds[0].events = POLLOUT;
        } else {
          fds[0].events = POLLIN | POLLOUT;
        }
      }
    } else if (fds[0].revents & POLLOUT) {
      write_len = write(fds[0].fd, buffer, length);
      syslog(LOG_INFO, "Write length: %lu\n", write_len);
      if (write_len < length) {
        if (write_len > 0) {
          memmove(buffer, buffer+write_len, length - write_len);
          length -= write_len;
          total_write_len += write_len;
          fds[0].events = POLLIN | POLLOUT;
        }
      } else {
        length = 0;
        total_write_len += write_len;
        fds[0].events = POLLIN;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  pid_t ruid, euid, suid;
  gid_t rgid, egid, sgid;
  openlog("echoserver", LOG_NDELAY | LOG_PID, LOG_DAEMON);
  if (getresuid(&ruid, &euid, &suid) == -1) {
  syslog(LOG_ERR, "Failed to run getresuid\n");
  }
  syslog(LOG_INFO, "Running as real UID=%d, EUID=%d, SUID=%d\n",ruid, euid, suid);
  syslog(LOG_INFO, "Running as real GID=%d, EGID=%d, SGID=%d\n",rgid, egid, sgid);
  poll_events();
}
