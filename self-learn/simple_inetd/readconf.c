#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "common.h"
#include "readconf.h"


// Method one to generate variable argument list, using triple pointers.
int add_new_arg(char ***args_array, int array_size,
                int element_index, char *arg) {
  if (array_size <= element_index + 1) {
    int new_size = array_size + INCREASE_ARGS;
    *args_array = realloc(*args_array, new_size * sizeof(char *));
    if (*args_array == NULL) {
      syslog(LOG_ERR, "Failed to realloc memory for more arguments!\n");
      exit(EXIT_FAILURE);
    }
    array_size = new_size;
  }

  if (arg != NULL) {
          (*args_array)[element_index] = strdup(arg);
  } else {
          (*args_array)[element_index] = NULL;
  }

  return array_size;
}


// Method two to generate variable argument list.
int add_new_arg2(struct AppStruct *appstruct, int array_size,
                 int element_index, char *arg) {
  if (array_size <= element_index + 1) {
    int new_size = array_size + INCREASE_ARGS;
    appstruct->args = realloc(appstruct->args, new_size * sizeof(char *));
    if (appstruct->args == NULL) {
      syslog(LOG_ERR, "Failed to realloc memory for more arguments!\n");
      exit(EXIT_FAILURE);
    }
    array_size = new_size;
  }

  if (arg != NULL) {
    appstruct->args[element_index] = strdup(arg);
  } else {
    appstruct->args[element_index] = NULL;
  }
  // Increase argument number.
  appstruct->argnum += 1;

  return array_size;
}


// Add a new AppStruct element to list appstructs.
// Return size of AppStruct array appstructs
int add_new_appstruct(struct AppStruct **appstructs, int array_size,
                      int element_index, char *user, char *app, int port) {
  if (array_size <= element_index + 1) {
    int index, new_size;
    struct AppStruct *newstructs;
    struct AppStruct *oldstructs = *appstructs;

    new_size = (array_size > 0) ? 2 * array_size : MIN_STRUCTS;
    newstructs = calloc(new_size, sizeof(struct AppStruct));

    // Copy old data to newstructs array.
    for (index = 0; index < element_index; index++) {
      // Struct copy will copy element pointers too.
      newstructs[index] = oldstructs[index];
    }
    // Allocate memory for arguments, default size is MIN_ARGS.
    for (index = element_index; index < new_size; index++) {
      newstructs[index].args = calloc(MIN_ARGS, sizeof(char *));
      newstructs[index].argnum = 0;
    }
    // Point to new list of AppStruct.
    *appstructs = newstructs;
    array_size = new_size;
    // First time reach here oldstructs will be NULL.
    if (oldstructs != NULL) free(oldstructs);
  }

  (*appstructs)[element_index].port = port;
  (*appstructs)[element_index].user = strdup(user);
  (*appstructs)[element_index].app = strdup(app);
  return array_size;
}


// Return a list of InetStructs from a list of AppStructs.
struct InetStruct *generate_inetstructs(struct AppStruct *appstructs,
                                        int number_of_apps) {
  int i;
  struct InetStruct *inetstructs;
  inetstructs = calloc(number_of_apps, sizeof(struct InetStruct));
  for (i = 0; i < number_of_apps; i++) {
    inetstructs[i].sock = -1;
    inetstructs[i].app = appstructs[i];
  }
  // Free appstructs
  free(appstructs);
  syslog(LOG_INFO, "Generated %d InetStruct elements\n", number_of_apps);
  return inetstructs;
}


// If two AppStruct is the same, return 1, or return -1
int compare_appstruct(struct AppStruct new_appstruct,
                      struct AppStruct old_appstruct) {
  int not_same = -1;
  int is_same = 1;
  if (new_appstruct.port != old_appstruct.port) {
    return not_same;
  }
  if (strcmp(new_appstruct.app, old_appstruct.app) != 0) {
    return not_same;
  }
  if (strcmp(new_appstruct.user, old_appstruct.user) != 0) {
    return not_same;
  }
  if (new_appstruct.argnum != old_appstruct.argnum) {
    return not_same;
  }
  int index;
  // Last argument is a NULL pointer, need to skip it, or strcmp will fail.
  for (index = 0; index < new_appstruct.argnum - 1; index++) {
    if (strcmp(new_appstruct.args[index], old_appstruct.args[index]) != 0) {
      return not_same;
    }
  }
  return is_same;
}


// Locate index of a AppStruct in a list of AppStruct with port.
// If there is a AppStruct with port, then return index or -1.
int search_for_port(struct AppStruct *appstructs, int size, int port) {
  int index;
  int not_found = -1;
  for (index = 0; index < size; index++) {
    if (port == appstructs[index].port) {
      return index;
    }
  }
  return not_found;
}


// Locate index of a InetStruct in a list of InetStruct.
// If app, port is same, then return index or -1.
int search_for_inet(struct InetStruct inetstruct,
                    struct InetStruct *inetstructs, int size) {
  int index;
  int not_found = -1;
  struct AppStruct source_app, target_app;
  source_app = inetstruct.app;
  for (index = 0; index < size; index++) {
    target_app = inetstructs[index].app;
    if (source_app.port == target_app.port &&
        strcmp(source_app.app, target_app.app) == 0) {
      return index;
    }
  }
  return not_found;
}


// If a AppStruct doesn't change in new InetStruct, keep the old socket.
// If AppStruct changes in new InetStruct, sock will be -1,
// which makes sure the socket will be recreated in inetd.
struct InetStruct *verify_inetstructs(struct InetStruct *new_inetstructs,
                                      struct InetStruct *old_inetstructs,
                                      int new_number_of_apps,
                                      int number_of_apps) {
  int new_index, old_index;
  struct InetStruct *inetstruct, *old;
  for (new_index = 0; new_index < new_number_of_apps; new_index++) {
    inetstruct = &(new_inetstructs[new_index]);
    old_index = search_for_inet(*inetstruct, old_inetstructs, number_of_apps);
    if (old_index != -1) {
      old = &(old_inetstructs[old_index]);
      if (compare_appstruct(inetstruct->app, old->app) > 0) {
        inetstruct->sock = old->sock;
        old->sock = -1;
        syslog(LOG_INFO, "Keep connected sock for app->%s, port->%d\n",
               inetstruct->app.app, inetstruct->app.port);
      }
    }
  }

  // Close all old sockets, as they are not in new config file.
  for (old_index = 0; old_index < number_of_apps; old_index++) {
    old = &(old_inetstructs[old_index]);
    if (old->sock != -1) {
      close(old->sock);
      old->sock = -1;
    }
  }

  return new_inetstructs;
}


// Parse config file to generate list of InetStruct inetd will take care.
// Caller function need to free the appstructs after using it.
struct InetStruct *parse_config(char *path, int *number_of_apps) {
  FILE *stream;
  int port_num;
  int arg_index;
  int app_index = 0;
  int size_of_args;
  int size_of_appstructs = 0;
  char buffer[BUFF_SZIE];
  char *portstr, *endptr,*app;
  char *user, *name, *arg, *newline;
  struct AppStruct *appstructs = NULL;


  stream = fopen(path, "r");
  if (stream == NULL) {
    syslog(LOG_ERR, "Failed to open file: %s\n", path);
    exit(EXIT_FAILURE);
  }

  syslog(LOG_INFO, "Start to load config from file: %s\n", path);

  // Begin parse config file.
  while ((fgets(buffer, BUFF_SZIE, stream)) != NULL) {
    arg_index = 0;
    size_of_args = MIN_ARGS;

    // Ignore comment lines.
    if (buffer[0] == '#') continue;

    // Remove the unnecessary newline character.
    if ((newline = strchr(buffer, '\n'))!= NULL) *newline = '\0';

    // Parse port, app and name, empty line if port == NULL.
    if ((portstr = strtok(buffer, TOK_DELIM)) == NULL ) continue;

    // TODO: Change them to regex, strtok is not a good solution.
    user = strtok(NULL, TOK_DELIM);
    app = strtok(NULL, TOK_DELIM);
    name = strtok(NULL, TOK_DELIM);

    // Each config line should at least contain port, user, app and name.
    if (user == NULL || app == NULL || name == NULL) {
      syslog(LOG_ERR, "Wrong format in config %s\n", path);
      exit(EXIT_FAILURE);
    }

    // Make sure we get a valid port number.
    port_num = parse_number(portstr, endptr, MAX_PORT);
    if (port_num < 0) {
      syslog(LOG_ERR, "Skipped port: %s,  not number! (app->%s, name->%s).\n",
             portstr, app, name);
      continue;
    }

    // Ignore the entry if already the port is already listened by another app.
    if (search_for_port(appstructs, size_of_appstructs, port_num) != -1) {
      syslog(LOG_ERR, "Skipped port: %s, duplicated! (app->%s, name->%s).\n",
             portstr, app, name);
      continue;
    }

    // Start to build list of AppStruct.
    size_of_appstructs = add_new_appstruct(&appstructs, size_of_appstructs,
                                           app_index, user, app, port_num);
    size_of_args = add_new_arg2(&(appstructs[app_index]),size_of_args,
                               arg_index, name);
    while ((arg = strtok(NULL, TOK_DELIM)) != NULL) {
      arg_index++;
      size_of_args = add_new_arg2(&(appstructs[app_index]),
                                  size_of_args, arg_index, arg);
    }
    add_new_arg2(&(appstructs[app_index]), size_of_args, ++arg_index, NULL);
    syslog(LOG_INFO, "Added one app: app->%s, port->%d\n", app, port_num);
    app_index++;
  }

  *number_of_apps = app_index;
  return generate_inetstructs(appstructs, *number_of_apps);
}


#if 0
int main(int argc, char *argv[]) {
  int i, num;
  int j;
  struct AppStruct *appstructs;
  appstructs = parse_config("config", &num);
  for (i = 0; i < num; i++) {
    printf("[%d]:name-> %s, port->%d\n", i,
           appstructs[i].app, appstructs[i].port);
    for (j = 0; j < 4; j++) {
      printf("appstructs[%d]->args[%d]=%s ", i, j, appstructs[i].args[j]);
    }
    printf("\n");
  }
}
#endif // 0
