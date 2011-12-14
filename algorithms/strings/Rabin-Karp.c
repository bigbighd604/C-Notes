#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Implement Rabin-Karp substring match algorithm.
 *
 * http://en.wikipedia.org/wiki/Rabin-Karp_string_search_algorithm
 *
 */

int search(const char *haystack, const char *needle) {
  int hash_needle = 0;
  int hash_rabin = 0;
  int position = 0;
  int needle_length = strlen(needle);
  int haystack_length = strlen(haystack);

  if (haystack_length < needle_length) {
    return -1;
  }

  while (*needle != '\0') {
    hash_needle += *(needle++);
  }

  int i = 0;
  int before_position = 0;
  int after_position = needle_length;

  for (i = 0; i < needle_length; i++) {
    hash_rabin += haystack[i];
  }

  while (after_position <= haystack_length) {
   // printf("hash_rabin:%d, hash_needle:%d\n", hash_rabin, hash_needle);
    if (hash_rabin == hash_needle) {
     // printf("before_position: %d", before_position);
      for (i = 0; i++; i < needle_length) {
    //    printf("%c, %c", haystack[before_position + i], needle[i]);
        if (haystack[before_position + i] != needle[i]) {
          break;
        }
      }
      return before_position;
    } else {
      //printf("after_position:%d, before_position:%d\n", after_position, before_position);
      //printf("after:%c, before:%c\n", haystack[after_position], haystack[before_position]);
      hash_rabin = hash_rabin + haystack[after_position] - haystack[before_position];
    }
    before_position++;
    after_position++;
  }
  return -1;
}

int main(int argc, char *argv[]) {
  char *str1 = argv[1];
  char *str2 = argv[2];
  int position = 0;
  position = search(str1, str2);
  if (position != -1) {
    printf("Found at index: %d\n", position);
  } else {
    printf("Not found!\n");
  }

}
