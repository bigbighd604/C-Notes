#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *fp;
  char *txt = "test";

  fp = fopen("./test", "w");
  fseek(fp, 1000000, 0);
  fwrite(txt, 4, sizeof(*txt), fp);
  fclose(fp);

}
