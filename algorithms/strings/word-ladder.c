#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>


#define LINE_BUFF_SIZE (1024)
#define HASH_SIZE (1024 * 1024)
#define INIT_ARRAY_SIZE (256)


struct neighbor {
  char *word;
  int is_visited;
  struct neighbor **neighbors;
};


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


char **get_dynamic_array(char ***old_array, size_t old_size, size_t new_size) {
  char **new_array;
  new_array = malloc(sizeof(char *) * new_size);

  if (old_array != NULL) {
    memcpy(new_array, *old_array, old_size);
  }
  return new_array;
}


void add_word_to_dynamic_array(char *word, char ***array, int *size, int *word_nums) {
  if (*word_nums >= *size - 1) {
    int new_size = *size * 2;
    *array = get_dynamic_array(array, *size, new_size);
    *size = new_size;
  }
  *array[*word_nums] = strdup(word);
  (*word_nums)++;
}


char **get_variants(const char *word, char *chars) {
  int i, length;
  char letter;
  char *copy_word, *pointer;
  char **neighbor_array;
  int neighbor_nums = 0;
  int array_size = INIT_ARRAY_SIZE;
  ENTRY e, *ep;

  length = strlen(word);
  copy_word = strdup(word);
  neighbor_array = get_dynamic_array(NULL, 0, array_size);

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
      if (ep != NULL){
        printf("%s\n", copy_word);
        add_word_to_dynamic_array(copy_word, &neighbor_array, &array_size, &neighbor_nums);
      }
      pointer++;
    }
    copy_word[i] = letter;
  }
  return neighbor_array;
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
