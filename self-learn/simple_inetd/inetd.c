#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "common.h"
#include "readconf.h"

volatile static sig_atomic_t got_a_sighup = 0;


// Handle SIGHUP, reset global variable to mark a config need reload.
void mark_reload_config(int signum) {
  got_a_sighup = 1;
}


// Fork a child and execv corresponding application.
void folk_and_forget(int conn, struct AppStruct appstruct) {
  pid_t pid;

  pid = fork();
  if (pid < 0) {
    syslog(LOG_ERR, "fork: %s", strerror(errno));
    close(conn);
  } else if (pid > 0) {
    close(conn);
    syslog(LOG_INFO, "Child  with pid %d is forked.\n", pid);
  } else {
    int i;
    for (i = 0; i < 3; i++) {
      dup2(conn, i);
    }
    for (i = 3; i <= sysconf(_SC_OPEN_MAX); i++) {
      close(i);
    }
    close(conn);
    lower_privilege(appstruct.user);
    execv(appstruct.app, appstruct.args);
    syslog(LOG_ERR, "execv: %s", strerror(errno));
    _exit(EXIT_FAILURE);
  }
}


// Poll all listening socks for POLLIN events.
// If there is new connection, accept and call handle_connection
void poll_listen_socks(struct InetStruct *inetstructs, int number_of_apps) {
  int index;
  int conn;
  struct pollfd fds[number_of_apps];
  for (index = 0; index < number_of_apps; index++) {
    fds[index].fd = inetstructs[index].sock;
    fds[index].events = POLLIN;
  }

  while (1) {
    // TODO: investigate epoll later
    if ((poll(fds, number_of_apps, -1)) < 0) {
      syslog(LOG_ERR, "poll: %s", strerror(errno));

      // Got SIGHUP signal, stop loop to reload config file.
      if ((errno == EINTR) && (got_a_sighup != 0)) {
        break;
      } else {
        continue;
      }
    }
    for (index = 0; index < number_of_apps; index++) {
      struct pollfd *fd = &(fds[index]);
      if (fd->revents & POLLIN) {
        struct sockaddr_in peer;
        socklen_t size = sizeof(peer);

        if ((conn = accept(fd->fd, (struct sockaddr *)&peer, &size)) == -1) {
          syslog(LOG_ERR, "accept: %s", strerror(errno));
        } else {
          folk_and_forget(conn, (inetstructs[index]).app);
        }
      }
    }

    // Got a SIGHUP, need to reload config.
    if (got_a_sighup != 0) break;
  }
}


// Generate a single socket and listen on port.
int generate_a_socket(int port, char *app) {
  // Generate a listen socket on port.
  int sock;
  int optval = 1;
  struct sockaddr_in listen_addr;
  sock = socket(AF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
  if (sock == -1) {
    syslog(LOG_ERR, "socket: %s", strerror(errno));
    syslog(LOG_ERR, "Failed to create socket for %s on port %d\n", app, port);
    exit(EXIT_FAILURE);
  }

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(optval));

  // TODO: Change to getaddrinfo
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(port);
  listen_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) != 0) {
    syslog(LOG_ERR, "bind: %s", strerror(errno));
    syslog(LOG_ERR, "Failed to bind %d to %s\n", port, app);
    exit(EXIT_FAILURE);
  }

  if (listen(sock, 2) != 0) {
    syslog(LOG_ERR, "listen: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  syslog(LOG_INFO, "App->%s is listening %s:%d on fd %d\n", app,
         inet_ntoa(listen_addr.sin_addr), port, sock);
  return sock;

}


// Generate listen sockets for all defined apps if there isn't one
struct InetStruct *generate_listen_socks(struct InetStruct *inetstructs,
                                         int number_of_apps) {
  int i;
  struct AppStruct *appstruct;

  for (i = 0; i < number_of_apps; i++) {
    if (inetstructs[i].sock == -1) {
      appstruct = &((inetstructs[i]).app);
      inetstructs[i].sock = generate_a_socket(appstruct->port, appstruct->app);
    }
  }
  return inetstructs;
}


int main(int argc, char * argv[]) {
  int number_of_apps;
  char *name = "simple_inetd";
  struct InetStruct *inetstructs = NULL;

  if (argc < 2) {
    printf("Usage: %s config-file\n", argv[0]);
    exit(EXIT_FAILURE);
  }


  // After call this function, use syslog to generate troubleshoot info.
  daemonize(name);

  // Register exit clean up function.
  atexit(clean_pidfile);

  // Register handler for SIGCHLD and SIGTERM
  register_handler(SIGCHLD, clean_children);
  register_handler(SIGTERM, clean_and_exit);
  register_handler(SIGHUP, mark_reload_config);

  // Poll listen socks and generate child to handle new connections.
  while (1) {
    if (inetstructs == NULL) {
      inetstructs = parse_config(argv[1], &number_of_apps);
    }

    // Got SIGHUP, try to reload config file.
    if (got_a_sighup > 0) {
      got_a_sighup = 0;
      int new_number_of_apps;
      struct InetStruct *new_inetstructs;
      new_inetstructs = parse_config(argv[1], &new_number_of_apps);

      // Preserve existing sockets if config doesn't change for that app.
      verify_inetstructs(new_inetstructs, inetstructs,
                         new_number_of_apps, number_of_apps);

      // Free old InetStruct list.
      free(inetstructs);

      // Assign new InetStruct list.
      inetstructs = new_inetstructs;
      number_of_apps = new_number_of_apps;
    }

    inetstructs = generate_listen_socks(inetstructs, number_of_apps);
    poll_listen_socks(inetstructs, number_of_apps);
  }
}
