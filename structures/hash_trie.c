#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 256


struct hashtable {
  int uid;
  char *username;
  struct hashtable *next;
};


static struct hashtable *table[HASH_SIZE];


static int hash(const char *key) {
  int hash_value = 0;
  for (; *key != '\0'; key++) {
    hash_value += *key;
  }
  return hash_value % HASH_SIZE;
}


static struct hashtable *lookup(struct hashtable **table, const char *key) {
  struct hashtable *entry;
  entry = table[hash(key)];
  //printf("Hash Key for %s is: %d\n", key, hash(key));
  for (entry; entry != NULL; entry = entry->next) {
    //printf("Hash Key for %s is: %d\n", key, hash(key));
    if (strcmp(entry->username, key) == 0) {
      return entry;
    }
  }
  return entry;
}


static void insert(struct hashtable **table, const char *key, int uid) {
  struct hashtable *entry;
  if (lookup(table, key) == NULL) {
    entry = calloc(1, sizeof(*entry));
    entry->username = strdup(key);
    entry->uid = uid;
    entry->next = table[hash(key)];
    table[hash(key)] = entry;
    //printf("Hash Key for %s is: %d\n", key, hash(key));
  }
#if 0
  struct hashtable *entry, *node, *new_entry;
  entry = table[hash(key)];
  if (entry != NULL) {
    for (node = entry; node != NULL; node = node->next) {
      if (strcmp(node->username, key) == 0) {
        node->uid = uid;
      }
    }
    new_entry = calloc(1, sizeof(*entry));
    new_entry->username = strdup(key);
    new_entry->uid = uid;
    new_entry->next = entry->next;
    entry->next = new_entry;
  } else {
    new_entry = calloc(1, sizeof(*entry));
    new_entry->username = strdup(key);
    new_entry->uid = uid;
    new_entry->next = NULL;
    table[hash(key)] = new_entry;
  }
#endif
}


int main(int argc, char *argv[]) {
  char line[1024], user[sizeof(line)];
  int uid;
  struct hashtable *entry;

  memset(table, 0, sizeof(table));

  while(fgets(line, sizeof(line), stdin) != NULL) {
    if (sscanf(line, "%[^:]:%*[^:]:%d", user, &uid) == 2) {
//printf("User:%s, Uid: %d\n", user, uid);
      insert(table, user, uid);
    }
  }

  printf("-------------------------\n");
  if (argc > 1) {
    entry = lookup(table, argv[1]);
    if (entry == NULL) {
      printf("No such user: [%s]\n", argv[1]);
    }
    else {
      printf("Uid: [%d]\n", entry->uid);
    }
  }
  return EXIT_SUCCESS;
}
