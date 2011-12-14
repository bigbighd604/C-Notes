/*
 * =====================================================================================
 *
 *       Filename:  trie.c
 *
 *    Description:  trie to load /etc/passwd
 *
 *        Version:  1.0
 *        Created:  10/18/2011 01:59:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hongli Lv (), andrewlv@google.com
 *        Company:  Google Inc.
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct trie {
  int uid;
  char *user;
  struct trie *tab[256];
};

static void insert(struct trie *root, const char *key, int uid) {
  int ch = *key;

  if (ch == '\0') {
    root->uid = uid;
  } else {
    if (root->tab[ch] == NULL) {
      root->tab[ch] = calloc(1, sizeof(struct trie));
    }
    insert(root->tab[ch], key + 1, uid);
  }
}


static const struct trie *lookup(const struct trie *root, const char *key) {
  int ch = *key;

  if (ch == '\0') {
    return root;
  } else if (root->tab[ch] == NULL) {
    return NULL;
  } else {
    return lookup(root->tab[ch], key + 1);
  }
}

int main(int argc, char *argv[]) {
  struct trie root;
  char line[1024], user[sizeof(line)];
  int uid;
  const struct trie *trie;

  memset(&root, 0, sizeof(root));

  while(fgets(line, sizeof(line), stdin) != NULL) {
    if (sscanf(line, "%[^:]:%*[^:]:%d", user, &uid) == 2) {
      insert(&root, user, uid);
    }
  }

  if (argc > 1) {
    trie = lookup(&root, argv[1]);
    if (trie == NULL) {
      printf("No such user: [%s]\n", argv[1]);
    }
    else {
      printf("Uid: [%d]\n", trie->uid);
    }
  }
  return EXIT_SUCCESS;
}
