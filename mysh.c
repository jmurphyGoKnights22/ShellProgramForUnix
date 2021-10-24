#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE !TRUE
#define ASCII_SPACE 32

int main(void) {
  char str[120];
    while (TRUE) {
      printf("# ");
      fgets(str, 120, stdin);
      char* token = strtok(str, " \n");

      while (token != NULL) {
        printf("%s ", token);
        token = strtok(NULL, " \n");
        switch (token) {
          case 'movetodir' :
            movetodir();
            break;
          case 'whereami' :
            whereami();
            break;
          case 'history' :
            history();
            break;
          case 'byebye' :
            byebye();
            break;
          case 'replay' :
            replay();
            break;
          case 'start' :
            start();
            break;
          case 'background' :
            background();
            break;
          case 'dalek' :
            dalek();
            break;
          case 'repeat' :
            repeat();
            break;
          case 'dalekall' :
            dalekall();
            break;
        }
      }
      printf("\n");
  }
  
  return 0;
}
void movetodir() {

}

void whereami() {

}

void history() {

}

void byebye() {

}

void replay() {

}

void start() {

}

void background() {

}

void dalek() {

}

void repeat() {

}

void dalekall() {

}