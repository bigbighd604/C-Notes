#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
  char *ptr = "Cisco Systems";
  *ptr++;
  printf("*ptr++ = %s\n", ptr); // "isco System"
  ptr++;
  printf("ptr++ = %s\n", ptr); // "sco System"

}
