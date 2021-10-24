#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE !TRUE
#define ASCII_SPACE 32

void movetodir(char* token);
void whereami();
void history(int flag);
void byebye();
void replay(int num);
void start();
void background();
void dalek(int num);
void repeat();
void dalekall();

int main(void) {
  char str[120];

  while (TRUE) {
    printf("# ");
    fgets(str, 120, stdin); // take input from stdin until newline reached, store in str
    char* token = strtok(str, " \n"); // tokenizes input from stdin (str), delimiting by space or newline characters

    if (strcmp(token, "movetodir") == 0) {
      token = strtok(NULL, " \n");
      movetodir(token); // "move" to directory specified by argument saved in token
    }
    else if (strcmp(token, "whereami") == 0) {
      whereami(); // print the value of the currentdir variable
    }
    else if (strcmp(token, "history") == 0) {
      token = strtok(NULL, " \n");
      if (token != NULL) {
        history(TRUE);  // '-c' flag included, clear history list
      }
      else {
        history(FALSE); // '-c' flag not included, print history list
      }
    }
    else if (strcmp(token, "byebye") == 0) {
      byebye(); // save history contents to a file
      break;  // exit the loop
    }
    else if (strcmp(token, "replay") == 0) {  // TODO: check if argument not included or not a number (invalid input)
      token = strtok(NULL, " \n");
      int num = atoi(token);
      replay(num); // re-execute the command labeled with number in the history
    }
    else if (strcmp(token, "start") == 0) { // TODO: WIP function, must support arbitrary parameters
      start();
    }
    else if (strcmp(token, "background") == 0) {  // TODO: WIP function as above
      background();
    }
    else if (strcmp(token, "dalek") == 0) { // TODO: check if argument not included or not a number (invalid input)
      token = strtok(NULL, " \n");
      int num = atoi(token);
      dalek(num); // Immediately terminate the program with the specific PID
    }
    else if (strcmp(token, "repeat") == 0) {    // Extra credit function
      repeat();
    }
    else if (strcmp(token, "dalekall") == 0) {  // Extra credit function
      dalekall();
    }
    else {
      printf("Invalid command, please check your input and try again.");
    }

    printf("\n");
  }
  
  return 0;
}
void movetodir(char* token) {
  printf("movetodir %s", token);
}

void whereami() {
  printf("whereami");
}

void history(int flag) {
  if (flag == TRUE) {
    printf("history cleared");
  }
  else {
    printf("history listed");
  }
}

void byebye() {
  printf("have a good day :)");
}

void replay(int num) {
  printf("replay %d", num);
}

void start() {
  printf("start");
}

void background() {
  printf("background");
}

void dalek(int num) {
  printf("dalek %d", num);
}

void repeat() {
  printf("repeat");
}

void dalekall() {
  printf("dalekall");
}