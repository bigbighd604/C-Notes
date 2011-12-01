#include <stdio.h>
#include <string.h>

/*
 * Penny Game: http://www.spoj.pl/problems/NY10A
 */

int main(int argc, char *argv[]) {
  int pairs, index;
  int i, j, k;
  char *p;

  scanf("%d", &pairs);
  char str[pairs][41];

  for (i = 0; i < pairs; i++) {
    scanf("%d", &index);
    scanf("%s", str[i]);
  }

  char *matches[8] = {
    "TTT", "TTH", "THT", "THH", "HTT", "HTH", "HHT", "HHH"
  };

  int counts[8];

  for (k = 0; k < pairs; k++) {
    for (i = 0; i < 8; i++) {
      counts[i] = 0;
    }
    for (i = 0; i < 38; i++) {
      for (j = 0; j < 8; j++) {
        if (memcmp(matches[j], str[k]+i, 3) == 0) {
          counts[j]++;
          break;
        }
      }
    }
    printf("%d", k + 1);
    for (j = 0; j < 8; j++) printf(" %d", counts[j]);
    printf("\n");
  }
  return 0;
}



