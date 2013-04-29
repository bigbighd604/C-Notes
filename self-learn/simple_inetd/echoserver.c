#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE (1 << 20)

void do_nothing(int signum) {
  int signumber = signum;
}


int main(int argc, char * argv[]) {
  signal(SIGALRM, &do_nothing);
  // port under 1024 are priviledged ports, only process having
  // the CAP_NET_BIND_SERVICE capability can bind. (man 7 ip)
  int port = 1026;
  char buffer[BUFF_SIZE];
  struct sockaddr_in listen_addr;
  int sock, conn;

  // protocol can be 0 for the following 2 situations: (man 7 ip)
  // IPROTO_TCP for TCP sockets and IPROTO_UDP for UDP sockets.
  sock = socket(AF_INET, SOCK_STREAM, 0);
  // sock = socket(AF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
  if (sock == -1) {
    perror("socket:");
    exit(1);
  }

  // address and port are always stored in network byte order.
  // All address/port manipulateion functions in the standard library
  // work in network bype order.
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(port); // translate to network byte order.
  listen_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) != 0) {
    perror("bind:");
    exit(2);
  }

  // If there is no room in the queue for new connections, no RST will be sent
  // allowing the client to automatically continue trying to connect by
  // retransmitting SYN.
  // If set it bigger than /proc/sys/net/core/somaxconn, will be truncated.
  if (listen(sock, 2) != 0) {
    perror("listen:");
    exit(3);
  }
  printf("listeningg on %s:%d\n", inet_ntoa(listen_addr.sin_addr), port);


  while (1) {
    struct sockaddr_in peer;
    socklen_t size = sizeof(peer);

    if ((conn = accept(sock, (struct sockaddr *)&peer, &size)) == -1) {
      perror("accept:");
      continue;
    }

    printf("Got a connection from %s:%d\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port));

    while (1) {
      ssize_t read_len = 0;
      read_len = read(conn, buffer, BUFF_SIZE);
      printf("Read length: %d\n", (int)read_len);
      if (read_len > 0) {
        ssize_t write_len = write(conn, buffer, strlen(buffer));
        if ( write_len < strlen(buffer)) {
          fprintf(stderr, "failed to write response to sock %d.\n", conn);
        }
      } else if (read_len == 0) {
        close(conn);
        printf("closed connection\n");
        break;
      } else {
        perror("read:");
      }
      memset(buffer, 0, read_len);
      read_len = 0;
    }
  }
  close(sock);
}
