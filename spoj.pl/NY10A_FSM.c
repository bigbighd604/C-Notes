#include <stdio.h>
#include <string.h>

/*
 * Penny Game: http://www.spoj.pl/problems/NY10A
 * FSM version
 */

int main(int argc, char *argv[]) {
  int pairs, index;
  int i, j;
  char *p;

  scanf("%d", &pairs);
  char str[pairs][41];

  for (i = 0; i < pairs; i++) {
    scanf("%d", &index);
    scanf("%s", str[i]);
  }

  int counts[8];

  for (i = 0; i < pairs; i++) {
    for (j = 0; j < 8; j++) {
      counts[j] = 0;
    }
    
    if (memcmp("TT", str[i], 2) == 0) {
      p = str[i]+2;
      goto TT;
    } else if (memcmp("TH", str[i], 2) == 0) {
      p = str[i]+2;
      goto TH;
    } else if (memcmp("HT", str[i], 2) == 0) {
      p = str[i]+2;
      goto HT;
    } else if (memcmp("HH", str[i], 2) == 0) {
      p = str[i]+2;
      goto HH;
    }

    TT:
      switch (*p) {
        case 'T':
          counts[0]++;
          p++;
          goto TT;
        case 'H':
          counts[1]++;
          p++;
          goto TH;
        case '\0':
          goto END;
      }
    TH:
      switch (*p) {
        case 'T':
          counts[2]++;
          p++;
          goto HT;
        case 'H':
          counts[3]++;
          p++;
          goto HH;
        case '\0':
          goto END;
      }
    HT:
      switch (*p) {
        case 'T':
          counts[4]++;
          p++;
          goto TT;
        case 'H':
          counts[5]++;
          p++;
          goto TH;
        case '\0':
          goto END;
      }
    HH:
      switch (*p) {
        case 'T':
          counts[6]++;
          p++;
          goto HT;
        case 'H':
          counts[7]++;
          p++;
          goto HH;
        case '\0':
          goto END;
      }
    END:
      printf("%d", i + 1);
      for (j = 0; j < 8; j++) printf(" %d", counts[j]);
      printf("\n");
  }

  return 0;
}



