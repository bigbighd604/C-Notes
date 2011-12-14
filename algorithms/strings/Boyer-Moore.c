#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define ALPHABET_LEN 255
#define NOT_FOUND patlen
#define max(a, b) ((a < b) ? b : a)

/*
 * delta1 table: delta1[c] contains the distance between the last
 * character of pat and the rightmost occurence of c in pat.
 * If c does not occur in pat, then delta1[c] = patlen.
 * If c is at string[i] and c != pat[patlen-1], we can
 * safely shift i over by delta1[c], which is the minimum distance
 * needed to shift pat forward to get string[i] lined up with some
 * characters in pat.
 * This table is sometimes called the "bad character shift".
 * This algorithm runs in alphabet_len + patlen time.
 */
void make_delta1(int *delta1, uint8_t *pat, int32_t patlen) {
  int i;
  for (i = 0; i < ALPHABET_LEN; i++) {
    // will shift length of pattern, NOT_FOUND == patlen
    delta1[i] = NOT_FOUND;
  }
  for (i = 0; i < patlen - 1; i++) {
    // from left to right, make sure delta1[pat[i]] is the smallest distance
    // because pat[i] may appear multiple times in pattern
    // [patlen - 1] means we only record the calculation for patlen - 1 letters
    delta1[pat[i]] = patlen - 1 - i;
  }
}

/*
 * true if the suffix of word starting from word[pos] is a prefix of word
 */
int is_prefix(uint8_t *word, int wordlen, int pos) {
  int i;
  int suffixlen = wordlen - pos;
  for (i = 0; (i < suffixlen) && (word[i] == word[pos + i]); i++);
  // two stop conditions - mismatch, and reach the last character
  // of word (word[wordlen - 1]. we have a prefix if the latter occurs.
  return !(i < suffixlen);
}

/*
 * length of the longest suffix of word ending on word[pos].
 * suffix_length("dddbcabc", 8, 4) = 2.
 */
int suffix_length(uint8_t *word, int wordlen, int pos) {
  int i;
  // increment suffix length i to the first mismatch or beginning of the word.
  for (i = 0; (word[pos - i] == word[wordlen - 1 - i]) && (i < pos); i++);
  return i;
}

/*
 * delta2 table: given a mismatch at pat[pos], we want to align with the
 * next possible full match could be based on what we know about pat[pos+1]
 * to pat[patlen-1].
 *
 * In case 1:
 * pat[pos+1] to pat[patlen-1] does not occur elsewhere in pat, the next
 * plausible match starts at or after the mismatch.
 * If, within the substring pat[pos+1 .. patlen-1], lies a prefix of pat,
 * the next plausible match is here(if there are multiple prefix in the
 * substing, pick the longest). Otherwise, the next plausible match starts
 * past the character aligned with pat[patlen-1].
 *
 * In case 2:
 * pat[pos+1] to pat[patlen-1] does occur elsewhere in pat. The mismatch
 * tells us that we are not looking at the end of a match. We may, however,
 * be looking at the middle of a match.
 *
 * The first loop, which takes care of case 1, is analogous to the KMP table
 * adapted for a 'backwards' scan order with the additional restriction that
 * the substring it considers as potential prefixes are all suffixes. In the
 * worst case scenario, pat consits of the same letters repated, so every
 * suffix is a prefix. This loop alone is not sufficient, however:
 * Suppose that pat is "ABYXCDEYX", and text is ".....ABYXCDEYX".
 * We will match X, Y, and find B != E. There is no prefix of pat is the
 * suffix "YX", so the first loop tells us to skip forward by 9 characters.
 * Although superficially similar to the KMP table, the KMP table relies on
 * information about the beginning of the partial match that the Boyer-Moore
 * algorithm does not have.
 *
 * The second loop addresses case 2. Since suffix_length may not be unique,
 * we want to take the minimum value, which will tell us how far away the
 * closest potential match is.
 */
void make_delta2(int *delta2, uint8_t *pat, int32_t patlen) {
  int p;
  int last_prefix_index = patlen - 1;

  // first loop
  for (p = patlen - 1; p >= 0; p--) {
    if (is_prefix(pat, patlen, p+1)) {
      last_prefix_index = p + 1;
      printf("position: %d is prefix\n", p);
      printf("last_prefix_index: %d\n", last_prefix_index);
    } else {
      ++last_prefix_index;
      printf("position: %d is not a prefix\n", p);
      printf("last_prefix_index: %d\n", last_prefix_index);
    }
    delta2[p] = last_prefix_index;
  }

  // second loop
  for (p = 0; p < patlen - 1; p++) {
    int slen = suffix_length(pat, patlen, p);
    delta2[patlen - 1 - slen] = patlen - 1 - p + slen;
    printf("Longest suffix for position: %d is:%d\n", p, slen);
    printf("delta2[%d] = %d\n", patlen - 1 - slen, patlen - 1 - p + slen);
  }
}


char *boyer_moore(uint8_t *string, uint32_t stringlen, uint8_t *pat, uint32_t patlen) {
  int i;
  int delta1[ALPHABET_LEN];
  int *delta2 = malloc(patlen * sizeof(int));
  make_delta1(delta1, pat, patlen);
  make_delta2(delta2, pat, patlen);
  for (i = 0; i < patlen; i++) {
    printf("delta2[%d]=%d,", i, delta2[i]);
  }
  printf("\n");

  i = patlen - 1;
  while (i < stringlen) {
    int j = patlen - 1;
    while (j >= 0 && (string[i] == pat[j])) {
      --i; // Pay attention, i is reduced here.
      --j;
    }
    if (j < 0) {
      free(delta2);
      return (string + i + 1);
    }
    i += max(delta1[string[i]], delta2[j]);
  }
  free(delta2);
  return NULL;
}


int main(int argc, char *argv[]) {
  char *haystack = "BBCDABD ABCDABD ABCDABCDABDE";
  char *needle = "ABCDABD";
  int haystack_length = 23;
  int patlen = 7;
  char *found = boyer_moore(haystack, haystack_length, needle, patlen);
  if (found != NULL) {
    printf("Found:%s\n", found);
  }
  int *delta2 = malloc(8 * sizeof(int));
  char *pattern = "ANPANMAN";
  make_delta2(delta2, pattern, 8);
  int i;
  for (i = 0; i < 8; i++) {
    printf("delta2[%d]=%d,", i, delta2[i]);
  }
  printf("\n");
}
