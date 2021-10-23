#include <stdio.h>
#include <string.h>

int main(void) {
  printf("# ");
  char str[120];
  fgets(str, 120, stdin); // store line from stdin into str
  
  char* token = strtok(str, " ");

  while (token != NULL) {
    printf("%s\n", token);
    token = strtok(NULL, " ");
  }

  return 0;
}