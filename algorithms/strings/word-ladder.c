#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>


#define LINE_BUFF_SIZE (1024)
#define HASH_SIZE (1024 * 1024)


char *lowercase(char *word) {
  char *pointer;

  pointer = word;
  while (*pointer != '\0') {
    *pointer = tolower(*pointer);
    pointer++;
  }
  return word;
}


int heuristic(char *word1, char *word2) {
  int diff = 0;
  while (*word1 != '\0') {
    if (*word1 != *word2) diff++;
    word1++;
    word2++;
  }
  return diff;
}


char **get_variants(const char *word, char *chars) {
  int i, length;
  char letter;
  char *copy_word, *pointer;
  ENTRY e, *ep;

  length = strlen(word);
  copy_word = strdup(word);

  for (i = 0; i < length; i++) {
    letter = copy_word[i];
    pointer = chars;
    while (*pointer != '\0') {
      if (*pointer == letter) {
        pointer++;
        continue;
      };

      copy_word[i] = *pointer;
      e.key = copy_word;
      ep = hsearch(e, FIND);
      if (ep != NULL) printf("%s\n", copy_word);
      pointer++;
    }
    copy_word[i] = letter;
  }

}


void build_hashmap(FILE *stream) {
  ENTRY e, *ep;
  char line[LINE_BUFF_SIZE];
  while (fgets(line, LINE_BUFF_SIZE, stream) != NULL) {
    // fgets read newline, and append '\0' at the end. Remove it.
    line[strlen(line) - 1] = '\0';
    // copy it to a key.
    e.key = strdup(lowercase(line));
    ep = hsearch(e, ENTER);
    if (ep == NULL) {
      fprintf(stderr, "entry failed\n");
      exit(EXIT_FAILURE);
    }
  }
}


int main(int argc, char *argv[]) {
  char *dict_file;
  FILE *stream;
  ENTRY e, *ep;
  char *chars = "abcdefghijklmnopqrstuvwxyz";
  dict_file = argv[1];
  
  stream = fopen(dict_file, "r");
  if (stream != NULL) {
    if (hcreate(HASH_SIZE) > 0) {
      build_hashmap(stream);
    } else {
      exit(EXIT_FAILURE);
    }
  } else {
    exit(EXIT_FAILURE);
  }

  e.key = "word";
  ep = hsearch(e, FIND);
  printf("word -> %s\n", ep ? ep->key: "NULL");

  e.key = "Tamera";
  ep = hsearch(e, FIND);
  printf("Tamera -> %s\n", ep ? ep->key: "NULL");

  e.key = "healthxxx";
  ep = hsearch(e, FIND);
  printf("healthxxx -> %s\n", ep ? ep->key: "NULL");

  get_variants("word", chars);
  get_variants("camera", chars);
  printf("%d\n", heuristic("word", "ward"));
  printf("%d\n", heuristic("camera", "xxxxxx"));
}
