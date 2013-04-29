#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFF_SIZE (1 << 20)
#define ARRAY_SIZE (1 << 10)


struct BufferNode {
  int fd;
  int length;
  char buffer[BUFF_SIZE];
};


struct BufferList {
  struct BufferNode *node;
  struct BufferList *next;
};


struct BufferNode *getbuffer(struct BufferList **head, int fd) {
  // it will always return a node, if not exists, create one and return.
  struct BufferList *pointer;
  pointer = *head;
  while (pointer != NULL) {
    if (pointer->node->fd == fd) return pointer->node;
    pointer = pointer->next;
  }
  struct BufferList *element;
  struct BufferNode *node;
  element = calloc(1, sizeof(struct BufferList));
  node = calloc(1, sizeof(struct BufferNode));
  node->fd = fd;
  node->length = 0;
  element->node = node;
  element->next = *head;
  *head = element;
  return element->node;
}


void delbuffer(struct BufferList **head, int fd) {
  // delete buffer when socket is closed
  struct BufferList *pre, *pointer;
  pointer = *head;
  while (pointer != NULL) {
    if (pointer->node->fd == fd) {
      if (pointer == *head) {
        *head = pointer->next;
        free(pointer);
      } else {
        pre->next = pointer->next;
        free(pointer->node);
        free(pointer);
        break;
      }
    }
    pre = pointer;
    pointer = pointer->next;
  }
}


int setup_sock(int port) {
  int sock;
  int optval = 1;
  struct sockaddr_in listen_addr;

  sock = socket(AF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);

  if (sock == -1) {
    perror("socket:");
    exit(1);
  }
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(optval));
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(port);
  listen_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) != 0) {
    perror("bind:");
    exit(2);
  }

  if (listen(sock, 2) != 0) {
    perror("listen:");
    exit(3);
  }
  printf("listeningg on %s:%d\n", inet_ntoa(listen_addr.sin_addr), port);
  return sock;
}


void polling() {

}


void echo(int sock) {
  int conn;
  int nfds = 1;
  struct pollfd *fd, *fds, *other_fds, *temp;
  struct BufferList *head = NULL;
  struct BufferNode *node = NULL;

  fds = calloc(ARRAY_SIZE, sizeof(struct pollfd));
  other_fds = calloc(ARRAY_SIZE, sizeof(struct pollfd));
  fds[0].fd = sock;
  fds[0].events = POLLIN;
  other_fds[0].fd = sock;
  other_fds[0].events = POLLIN;

  while (1) {
    ssize_t read_len = 0;
    int index = 1, other_index = 1;

    if ((poll(fds, nfds, -1)) < 0) {
      perror("poll:");
      continue;
    }

    for (; index < nfds; index++) {
      fd = &(fds[index]);
      if ((fd->revents & POLLHUP) || (fd->revents & POLLERR)) {
        delbuffer(&head, fd->fd);
        close(fd->fd);
        printf("Connection closed for fds[%d]: %d\n", index, fd->fd);
        continue;
      }

      node = getbuffer(&head, fd->fd);
      if (fd->revents & POLLIN) {
        int available_size = BUFF_SIZE - node->length;
        read_len = read(fd->fd, node->buffer+node->length, available_size);
        printf("Read length: %d from fds[%d]: %d\n", (int)read_len,
            index, fd->fd);
        if (read_len == 0) {
          if (node->length >0) {
            other_fds[other_index].fd = fds[index].fd;
            other_fds[other_index].events = POLLOUT;
            other_index++;
          } else {
            delbuffer(&head, fd->fd);
            close(fd->fd);
            printf("Connection closed for fds[%d]: fd %d\n", index, fd->fd);
          }
        } else if (read_len < 0) {
          perror("read:");
        } else {
          node->length += read_len;
          other_fds[other_index].fd = fds[index].fd;
          if (node->length == BUFF_SIZE) {
            other_fds[other_index].events = POLLOUT;
          } else {
            other_fds[other_index].events = POLLIN | POLLOUT;
          }
          other_index++;
        }
      } else if ((fd->revents & POLLOUT)) {
        ssize_t write_len = write(fd->fd, node->buffer, node->length);
        if (write_len < node->length) {
          if (write_len > 0) {
            memmove(node->buffer, node->buffer + write_len,
                    node->length - write_len);
            node->length -= write_len;
          }
          other_fds[other_index].fd = fds[index].fd;
          other_fds[other_index].events = POLLIN | POLLOUT;
          other_index++;
          printf("Failed to write all data to fds[%d]: %d\n", index, fd->fd);
        } else {
          node->length = 0;
        }
        if (node->length == 0) {
          other_fds[other_index].fd = fds[index].fd;
          other_fds[other_index].events = POLLIN;
          other_index++;
        }
      } else {
        other_fds[other_index] = fds[index];
        other_index++;
      }
    }

    if (fds[0].revents & POLLIN) {
      struct sockaddr_in peer;
      socklen_t size = sizeof(peer);

      if ((conn = accept(fds[0].fd, (struct sockaddr *)&peer, &size)) == -1) {
        perror("accept:");
      } else {
        printf("Connected from %s:%d - fds[%d]: %d\n",
            inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), other_index, conn);
        other_fds[other_index].fd = conn;
        other_fds[other_index].events = POLLIN;
        other_index++;
      }
    }

    if ((fds[0].events == POLLIN) && (other_index  == ARRAY_SIZE)) {
      fds[0].events = 0;
    } else if ((fds[0].events == 0) &&
               (other_index <= (ARRAY_SIZE / 10 * 8))) {
      printf("other_index = %d", other_index);
      fds[0].events = POLLIN;
    }

    temp = fds;
    fds = other_fds;
    other_fds = temp;
    nfds = other_index;
  }
}


int main(int argc, char * argv[]) {
  int sock;

  if (argc < 2) {
    printf("Usage: %s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  sock = setup_sock(atoi(argv[1]));
  echo(sock);
  close(sock);
}
