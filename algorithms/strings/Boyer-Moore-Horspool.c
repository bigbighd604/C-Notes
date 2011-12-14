#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>

/*
 * The constant UCHAR_MAX is assumed to contain the maximum value of the
 * input character type. Typically it's 255.
 * size_t is an unsigned type for representing sizes.
 * If your system doesn't have it, replace with unsigned int.
 *
 * Returns a pointer to the first occurrence of "needle" within "haystack"
 * or NULL if not found. Works like memmem()
 *
 * Note: In this example needle is a Cstring. The ending 0x00 will be cutoff
 * so you could call this example with
 * boyermoore_horspool_memmem(haystack, hlen, "abc", sizeof("abc"))
 */


const unsigned char *
boyermoore_horspool_memmem(const unsigned char* haystack, ssize_t hlen,
                           const unsigned char* needle, ssize_t nlen) {
  size_t index = 0;
  // Officially called: bad character shift.
  size_t bad_char_skip[UCHAR_MAX + 1];

  // Sanity checks on the parameters.
  if (nlen <= 0 || !haystack || !needle) {
    return NULL;
  }

  // Initialize the table to default value
  // When a character is encountered that doesn't occur in the needle,
  // we can safely skip ahead for the whole length of the needle
  for (index = 0; index <= UCHAR_MAX; index++) {
    bad_char_skip[index] = nlen;
  }

  // C arrays have the first byte at [0], therefore:
  // [nlen - 1] is the last byte of the array.
  size_t last = nlen - 1;

  // figure out smallest distance from rightmost character of every
  // occurence letter, only calculate nlen -1 elements
  for (index = 0; index < last; index++) {
    bad_char_skip[needle[index]] = last - index;
  }

  // Search the haystack, while the needle can still be within it.
  while (hlen >= nlen) {
    // Scan from the end of the needle.
    for (index = last; haystack[index] == needle[index]; index--) {
      // If the first byte matches, we've found a match.
      //if (index == 0) printf("Matched: %s\n", haystack);
      if (index == 0) return haystack;
    }
    // Below is another version of checking match, more concise.
    /*
    for (index = last; (index >= 0 && haystack[index] == needle[index]); index--);
    if (index == -1) return haystack;
    */

    // printf("Index is: %d\n", index); //After a match, index == -1
    // Otherwise, we need to skip some bytes and start again.
    // Note that here we are getting the skip value based on the last byte
    // of needle, no matter where we didn't match. So if the needle is:
    // "abcd" then we are skipping based on 'd' and that value will be 4,
    // and for "abcdd" we again skip on 'd' but the value will be only 1.
    // The alternative of pretending that the mismatched character was
    // the last character is slower in the normal case. (Eg. finding "abcd"
    // in "...azcd..." gives 4 by using 'd' but only 4-2==2 using 'z'.
    //
    // Actually, here is using last byte of haystack, not needle.
    // if using last byte of needle, you can not get correct answer.
    // i.e.
    // char *haystack = "BABCDABCDAB ABCDABCDABDE";
    // char *needle = "ABCD";
    // if using bad_char_skip[needle[last]], will get a match:
    // "ABCDABCDABDE", which is wrong. should be:
    // "ABCDABCDAB ABCDABCDABDE".
    // So the explanation in Wikipedia is wrong!
    hlen -= bad_char_skip[haystack[last]];
    haystack += bad_char_skip[haystack[last]];
  }
  return NULL;
}


int main(int argc, char *argv[]) {
  //char *haystack = "ABC ABCDAB ABCDABCDABDE";
  //char *needle = "ABCDABD";
  char *haystack = "BABCDABCDAB ABCDABCDABDE";
  char *needle = "ABCD";
  const unsigned char *found = boyermoore_horspool_memmem(
      haystack, strlen(haystack), needle, strlen(needle));
  if (found != NULL) {
    printf("Found:%s\n", found);
  }
}
