#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "common.h"


// Parse a number from a string and check whether it's in given range.
int parse_number(char *str, char *endptr, int max_limit) {
  int value;
  int base = 10;
  int no_number = -1;
  int out_of_range = -2;

  // To distinguish success/failure after call
  errno = 0;
  value = strtol(str, &endptr, base);

  // Check for out of range
  if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))) {
    return out_of_range;
  }

  // For other errors.
  if ((endptr == str) || (errno !=0 && value == 0)) {
    return no_number;
  }

  // If we got here, strtol() successfully parsed a number
  if (value > max_limit || value < 1) {
    return out_of_range;
  }

  return value;
}


// Check whether there is already a pid file exists.
int is_pidfile_existed() {
  if (access(PID_FILE, F_OK) == 0) {
    return 1;
  }
  return 0;
}


// Check whether there is another copy already running.
// Send a null signal using kill. this function returns true too
// if there is another process with the same pid as in pid file.
// Not a perfect solution.
int is_daemon_running() {
  int pid;
  int kill_result;
  if (!is_pidfile_existed()) {
    return PID_NOT_EXISTED;
  }
  pid = get_daemon_pid();
  if (pid < 0) return PID_NOT_EXISTED;
  kill_result = kill(pid, 0);
  if (kill_result == 0) {
    return 1;
  } else if (errno == EPERM) {
    return 1;
  } else if (errno == ESRCH) {
    return PID_NOT_EXISTED;
  }
  return PID_NOT_EXISTED;
}


// Get pid from a existing pid file.
int get_daemon_pid() {
  int pid_num, fd;
  char buffer[256];
  char *endptr;
  fd = open(PID_FILE, O_RDONLY);
  if (fd == -1) {
    syslog(LOG_ERR, "open:%s", strerror(errno));
    return PID_NOT_EXISTED;
  }
  if (read(fd, buffer, 256) > 0) {
    if ((pid_num = parse_number(buffer, endptr, INT_MAX)) < 0) {
      return PID_NOT_EXISTED;
    }
    return pid_num;
  }
  return PID_NOT_EXISTED;
}


// Write pid to pid file. Use fflush to force the write immediately.
// Or later redirect stdout to /dev/null will discard this line.
int write_daemon_pid(pid_t pid) {
  FILE *stream;
  stream = fopen(PID_FILE, "w+");
  if (stream == NULL) {
    syslog(LOG_ERR, "fopen:%s", strerror(errno));
    return -1;
  }
  if (fprintf(stream, "%d\n", pid) > 0) {
    if (fflush(stream) == 0) {
      return 1;
    }
    return -1;
  }
  return -1;
}


// Delete pid file when normal exit or receive SIGTERM.
void clean_pidfile(void) {
  if (is_pidfile_existed() > 0) {
    if (unlink(PID_FILE) == -1) {
      syslog(LOG_ERR, "unlink:%s", strerror(errno));
    }
  }
}


// Delete pid file when receive SIGTERM.
void clean_and_exit(int signo) {
  clean_pidfile();
  _exit(EXIT_SUCCESS);

}


// Function to handle SIGCHLD signal.
void clean_children(int signo) {
  int status;
  pid_t pid;

  while (1) {
    pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED);
    if (pid > 0) {
      if (WIFEXITED(status)) {
        syslog(LOG_INFO, "Child with pid %d exited, status=%d\n",
               pid, WEXITSTATUS(status));
      } else if (WIFSIGNALED(status)) {
        syslog(LOG_INFO, "Child with pid %d killed by signal %d\n",
               pid, WTERMSIG(status));
      } else if (WIFSTOPPED(status)) {
        syslog(LOG_INFO, "Child with pid %d stopped by signal %d\n",
               pid, WSTOPSIG(status));
      } else if (WIFCONTINUED(status)) {
        syslog(LOG_INFO, "Child with pid %d continued to execution\n", pid);
      }
    } else {
      break;
    }
  }
}


// Helper function for registering singal handlers.
void register_handler(int signum, void (*handler)(int)) {
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(signum, &sa, NULL) == -1) {
    syslog(LOG_ERR, "sigaction: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
}


// Make a process a maemon with a name.
// After call this function, only use syslog to generate troubleshoot info.
void daemonize(char *name) {
  pid_t pid, sid;

  pid = fork();
  if (pid < 0) {
    perror("fork:");
    exit(EXIT_FAILURE);
  } else if (pid > 0) {
    // Don't want to do clean up here.
    _exit(EXIT_SUCCESS);
  } else {
    int fd, daemon_pid;
    umask(0);

    // Become session leader.
    sid = setsid();
    if (sid == -1) {
      perror("setsid:");
      exit(EXIT_FAILURE);
    }

    // Fork again to become a session member.
    // Eliminate the chance to get controlling terminal.
    pid = fork();
    if (pid < 0) {
      perror("fork:");
      exit(EXIT_FAILURE);
    } else if (pid > 0) {
      exit(EXIT_SUCCESS);
    } else {
      // Write pid file
      if (is_daemon_running() < 0) {
        if (write_daemon_pid(getpid()) < 0) {
          printf("Cannot write pid to %s\n", PID_FILE);
          exit(EXIT_FAILURE);
        }
      } else {
        printf("Daemon is already running with pid %d!\n", get_daemon_pid());
        exit(EXIT_FAILURE);
      }

      // Redirect stdin, stdout and stderr to /dev/null.
      fd = open(NULL_FILE, O_RDONLY);
      if (fd != -1) {
        dup2(fd, 0);
        close(fd);
      }
      fd = open(NULL_FILE, O_RDWR);
      if (fd != -1) {
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
      }

      // Change current working directory to "/".
      if (chdir("/") == -1)  {
        perror("chdir:");
        exit(EXIT_FAILURE);
      }

      // Enable syslog for this daemon.
      openlog(name, LOG_NDELAY | LOG_PID, LOG_DAEMON);
      syslog(LOG_INFO, "%s is daemonized successfully!", name);

    }
  }
}


// This function will be called from child process to setuid and setgid.
void lower_privilege(char *user) {
  struct passwd *pwdent;

  pwdent = getpwnam(user);
  if (pwdent == NULL) {
    syslog(LOG_ERR, "getpwnam: %s", strerror(errno));
    _exit(EXIT_FAILURE);
  }

  // Set init groups for calling process.
  if (initgroups(pwdent->pw_name, pwdent->pw_gid) == -1) {
    syslog(LOG_ERR, "initgroups: %s", strerror(errno));
    _exit(EXIT_FAILURE);
  }

  // initgroups only set supplementary groups, not primary group
  // After initgroups, still need call setgid to update primary group
  if (setgid(pwdent->pw_gid) == -1) {
    syslog(LOG_ERR, "setgid: %s", strerror(errno));
    _exit(EXIT_FAILURE);
  } else {
    syslog(LOG_INFO, "setgid to %d", pwdent->pw_gid);
  }

  // Set user id to specified user to run app.
  if (setuid(pwdent->pw_uid) == -1) {
    syslog(LOG_ERR, "setuid: %s", strerror(errno));
    _exit(EXIT_FAILURE);
  } else {
    syslog(LOG_INFO, "setuid to %s", user);
  }

}
