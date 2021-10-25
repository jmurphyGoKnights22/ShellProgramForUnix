/*
+-----------------------------------------------------------------+
| COP-4600 - Homework 2 - Shell program                           |
| Professor Gerald Hensel                                         |
| Due 29 October, 2021                                            |
| Group project by: Derrick Keough & James Murphy                 |
|=================================================================|
| Teammate Contributions:                                         |
| Derrick:                                                        |
|   - Set up project skeleton (defines, tokenizing input, etc)    |
|   - Worked on movetodir, whereami, history, & byebye functions  | 
|                                                                 |
| James:                                                          |
|   - Set up github repository for version control                |
|   - Helped set up skeleton (function declarations, hello world) |
|   - Worked on replay, start, background, and dalek functions    |
+-----------------------------------------------------------------+
*/

// TODO: decide on implementing extra credit functions (really just based on if we have time)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

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

DIR* currentdir;
char* currentdirString;
char** historyFile;

int main(int argc, char* argv[]) {
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
    else if (strcmp(token, "start") == 0) { // TODO: WIP setup, must support arbitrary parameters
      start();
    }
    else if (strcmp(token, "background") == 0) {  // TODO: WIP setup as above
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


// Assigned: Derrick
void movetodir(char* token) {
  DIR* dir = opendir(token);
  if (dir) {
    currentdir = dir;
    currentdirString = token;
    printf("currentdir set to %s", token);
    closedir(dir);
  }
  else if (ENOENT == errno) {
    printf("Error: specified directory %s does not exist.", token);
  }
  else {
    printf("Error: movetodir command returned an unknown error, please try again");
  }
}

// Assigned: Derrick
void whereami() {
  printf("%s", currentdirString);
}

// Assigned: Derrick
void history(int flag) {
  if (flag == TRUE) {
    printf("history cleared");
  }
  else {
    printf("history listed");
  }
}

// Assigned: Derrick
void byebye() {
  printf("have a good day :)");
}

// Assigned: James
void replay(int num) {
  printf("replay %d", num);
}

// Assigned: James
void start() {
  printf("start");
}

// Assigned: James
void background() {
  printf("background");
}

// Assigned: James
void dalek(int num) {
  printf("dalek %d", num);
}

// Extra Credit function
void repeat() {
  printf("repeat");
}

// Extra Credit function
void dalekall() {
  printf("dalekall");
}