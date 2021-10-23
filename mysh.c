#include <stdio.h>
#include <string.h>

int main(void) {
  printf("# ");
  char str[120];
  fgets(str, 120, stdin); // store line from stdin into str
  char* rest = str; // copy to be used by strtok_r() to maintain context between successive calls
  char* token;

  while ((token = strtok_r(rest, " ", &rest))) {
    printf("%s\n", token);
  }

  return 0;
}