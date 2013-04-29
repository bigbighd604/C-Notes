#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_print(char *txt, char *delimiter, char *saveptr1) {
  char *token;
  token = strtok_r(txt, delimiter, &saveptr1);
  if (token != NULL) {
    txt = token + strlen (token) + 1;
    reverse_print(txt, delimiter, saveptr1);
    printf("%s\n", token);
  }
}


int main(int argc, char *argv[]) {
  char txt[] = "Line 1\nLine 2\nLine 3\nLine 4\nLine 5\n";
  char *token;
  char *remain_txt;
  char *delimiter = "\n";
  char *saveptr1;
  token = strtok_r(txt, delimiter, &saveptr1);
  if (token == NULL) {
    printf("%s", token);
  } else {
    printf("%s\n", token);
    remain_txt = token + strlen (token) + 1;
    reverse_print(remain_txt, delimiter, saveptr1);
  }
}
