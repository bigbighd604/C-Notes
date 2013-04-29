// Copyright 2011 Google Inc. All Rights Reserved.
// Author: andrewlv@google.com (Hongli Lv)

#ifndef _COMMON_H_
#define _COMMON_H_

#define PID_NOT_EXISTED (-1)
#define NULL_FILE "/dev/null"
#define PID_FILE "/var/run/simple_inetd.pid"


void lower_privilege(char *user);

extern void daemonize(char *name);
extern int is_daemon_running();
extern int write_daemon_pid(pid_t pid);

extern void clean_pidfile(void);
extern void clean_and_exit(int signo);

extern void reload_conf(int signo);
extern void clean_children(int signo);

extern int parse_number(char *str, char *endptr, int max_limit);

extern void register_handler(int signum, void (*handler)(int));

#endif  // _COMMON_H_
