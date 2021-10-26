/*
+-----------------------------------------------------------------+
| Homework 2 - Shell program                                      |
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
// dalekall() seems straightforward after implementing dalek(), repeat() might be more intensive

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>


#define TRUE 1
#define FALSE !TRUE
#define ASCII_SPACE 32
#define MAX_COMMAND_ARGS 10 // As per the document, "You may set a reasonable maximum on the number of command line arguments, but your shell should handle input lines of any length". 

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
void clearTemp();
void addSpaceToTemp();
void addTokenToTemp(char* str);
void addToHistoryArray();

DIR* currentdir;
char* currentdirString;
char* tempCommand;
char** historyArray = NULL;
int arrayCount = 0;

int main(int argc, char* argv[]) {
  char str[120];
  tempCommand = malloc(120 * sizeof(char));

  while (TRUE) {
    printf("# ");
    fgets(str, 120, stdin); // Takes input from stdin until newline reached, stores in str
    char* token = strtok(str, " \n"); // Tokenizes input from stdin (str), delimiting by space or newline characters

    if (strcmp(token, "movetodir") == 0) {
      addTokenToTemp("movetodir");  // Add first argument to tempCommand
      token = strtok(NULL, " \n");  // Get next argument in the command
      addSpaceToTemp();             // Add a space after the first token "movetodir"
      addTokenToTemp(token);        // Add the argument to tempCommand
      addToHistoryArray();          // All arguments added, tempCommand ready to be added to history array
      movetodir(token);             // "Move" to directory specified by argument saved in token
    }
    else if (strcmp(token, "whereami") == 0) {
      addTokenToTemp("whereami");
      addToHistoryArray();
      whereami(); // Print the value of the currentdirString variable
    }
    else if (strcmp(token, "history") == 0) {
      addTokenToTemp("history");
      token = strtok(NULL, " \n");
      if (token != NULL) {
        addSpaceToTemp();
        addTokenToTemp(token);
        addToHistoryArray();
        history(TRUE);  // '-c' Flag included, clear history list
      }
      else {
        addToHistoryArray();
        history(FALSE); // '-c' Flag not included, print history list
      }
    }
    else if (strcmp(token, "byebye") == 0) {
      addTokenToTemp("byebye");
      addToHistoryArray();
      byebye(); // Save history contents to a file
      break;  // Exit the loop
    }
    else if (strcmp(token, "replay") == 0) {
      addTokenToTemp("replay");
      token = strtok(NULL, " \n");
      if (token == NULL || (atoi(token) == 0)) {
        printf("Error: invalid input, expected a number after \"replay\".\n");
        clearTemp();
        continue;
      }
      else {
        addSpaceToTemp();
        addTokenToTemp(token);
        addToHistoryArray();
        int num = atoi(token);
        replay(num); // Re-execute the command labeled with number in the history
      }
    }
    else if (strcmp(token, "start") == 0) { // TODO: WIP setup, must support arbitrary parameters
      char* path = strtok(NULL, " \n");
      start();
    }
    else if (strcmp(token, "background") == 0) {  // TODO: WIP setup as above
      background();
    }
    else if (strcmp(token, "dalek") == 0) { // TODO: Check if argument not included or not a number (invalid input)
      addTokenToTemp("dalek");
      token = strtok(NULL, " \n");
      if (token == NULL || (atoi(token) == 0)) {
        printf("Error: invalid input, expected a number after \"dalek\".\n");
        clearTemp();
        continue;
      }
      else {
        addSpaceToTemp();
        addTokenToTemp(token);
        addToHistoryArray();
        int num = atoi(token);
        dalek(num); // Immediately terminate the program with the specific PID
      }
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


// Clear tempCommand variable so it can be used in other areas
void clearTemp() {
  strcpy(tempCommand, "");
}

// Concatenate a space character for use betwen two tokens
void addSpaceToTemp() {
  addTokenToTemp(" ");
}

// Add some string to the tempCommand variable
void addTokenToTemp(char* str) {
  strcat(tempCommand, str);
}

// Allocate space and add tempCommand's contents to the history array then clear tempCommand
void addToHistoryArray() {
  historyArray = (char**)realloc(historyArray, (arrayCount + 1) * sizeof(char*)); // dynamically assign space to the array
  historyArray[arrayCount++] = strdup(tempCommand); // add the entire command to the array
  clearTemp();  // clear tempCommand variable so it can be used elsewhere
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
void history(int flag) {  // TODO: load a history file upon starting the shell (if one is available)
  if (flag == TRUE) { // "-c" Flag encountered, clear history
    int i;
    for (i = arrayCount - 1; i >= 0; i--) {
      free(historyArray[i]);
    }
    free(historyArray);
    arrayCount = 0;
    printf("history cleared");
  }
  else {  // No "-c" flag, print history
    int i;
    if (arrayCount == 1) {  // history is clear, don't access free'd memory
      printf("0: history");
      return;
    }
    for (i = arrayCount - 1; i >= 0; i--) {
      if (i != 0) {
        printf("%d: %s\n", (arrayCount - i - 1), historyArray[i]);
      }
      else {
        printf("%d: %s", (arrayCount - i - 1), historyArray[i]);
      }
    }
  }
}

// Assigned: Derrick
void byebye() { // TODO: send the history to a file once implemented
  printf("have a good day :)\n");
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
  int result = kill(num, SIGKILL);
  if (result == 0){
    printf("\nPID: %d was successfully terminated.", num);
  }
  else {
    printf("\nSignal Failed. Error Number: %d", errno);
  }
}

// Extra Credit function
void repeat() {
  printf("repeat");
}

// Extra Credit function
void dalekall() {
  printf("dalekall");
}