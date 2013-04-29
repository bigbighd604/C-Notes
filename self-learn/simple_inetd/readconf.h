#ifndef _READCONF_H_
#define _READCONF_H_

#define BUFF_SZIE (1 << 10) // 1K buffer size
#define MIN_STRUCTS (2)  // Initial number of AppStructs when building app list
#define MIN_ARGS (2)  // Initial number of app arguments when building args
#define INCREASE_ARGS (4)  // Increase step for arguments.
#define MAX_PORT (65535)
#define CHAR_SIZE (sizeof(char))
#define CHAR_PSIZE (sizeof(char *))
#define TOK_DELIM " \t"


struct AppStruct {
  int port;
  char *user;
  char *app;
  char **args;
  int argnum;
};


struct InetStruct {
  int sock; // connected socket.
  struct AppStruct app;
};

extern struct InetStruct *parse_config(char *path, int *number_of_apps);

struct InetStruct *verify_inetstructs(struct InetStruct *new_inetstructs,
                                      struct InetStruct *old_inetstructs,
                                      int new_number_of_apps,
                                      int number_of_apps);
#endif
