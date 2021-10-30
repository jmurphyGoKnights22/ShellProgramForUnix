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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#include <unistd.h>
#include <sys/wait.h>
=======

#define TRUE 1
#define FALSE !TRUE
//// For use in start() and background() functions
#define MAX_COMMAND_ARGS 10 // As per the document, "You may set a reasonable maximum on the number of command line arguments, but your shell should handle input lines of any length".

void movetodir(char *token);
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
void addTokenToTemp(char *str);
void addToHistoryArray();
void getHistory();

DIR *currentdir;
FILE *fp;
char *currentdirString;
char *tempCommand;
char **historyArray = NULL;
int historyArrayCounter = 0;
char** parameterArray;
int parameterCount;

int main(int argc, char *argv[])
{
  char str[120];
  tempCommand = malloc(120 * sizeof(char));

  getHistory(); // try to load history from a history.txt file


  while (TRUE)
  {
    printf("# ");
    fgets(str, 120, stdin);           // Takes input from stdin until newline reached, stores in str
    char *token = strtok(str, " \n"); // Tokenizes input from stdin (str), delimiting by space or newline characters

    if (strcmp(token, "movetodir") == 0)
    {
      addTokenToTemp("movetodir"); // Add first argument to tempCommand
      token = strtok(NULL, " \n"); // Get next argument in the command
      addSpaceToTemp();            // Add a space after the first token "movetodir"
      addTokenToTemp(token);       // Add the argument to tempCommand
      addToHistoryArray();         // All arguments added, tempCommand ready to be added to history array
      movetodir(token);            // "Move" to directory specified by argument saved in token
    }
    else if (strcmp(token, "whereami") == 0)
    {
      addTokenToTemp("whereami");
      addToHistoryArray();
      whereami(); // Print the value of the currentdirString variable
    }
    else if (strcmp(token, "history") == 0)
    {
      addTokenToTemp("history");
      token = strtok(NULL, " \n");
      if (strcmp(token,"-c") == 0) {

        addSpaceToTemp();
        addTokenToTemp(token);
        addToHistoryArray();
        history(TRUE); // '-c' Flag included, clear history list
      }
      else if(token != NULL) {
        printf("Error: unkonwn parameter for command \"history\".");
      }
      else {
        addToHistoryArray();
        history(FALSE); // '-c' Flag not included, print history list
      }
    }
    else if (strcmp(token, "byebye") == 0)
    {
      addTokenToTemp("byebye");
      addToHistoryArray();
      byebye(); // Save history contents to a file
      break;    // Exit the loop
    }
    else if (strcmp(token, "replay") == 0)
    {
      addTokenToTemp("replay");
      token = strtok(NULL, " \n");
      if (token == NULL || (atoi(token) == 0))
      {
        printf("Error: invalid input, expected a number after \"replay\".\n");
        clearTemp();
        continue;
      }
      else
      {
        addSpaceToTemp();
        addTokenToTemp(token);
        addToHistoryArray();
        int num = atoi(token);
        replay(num); // Re-execute the command labeled with number in the history
      }
    }
    else if (strcmp(token, "start") == 0)
    { // TODO: WIP setup, must support arbitrary parameters
      addTokenToTemp("start");
      token = strtok(NULL, " \n");
      if (token == NULL)
      {
        printf("Error: invalid input, expected a path after \"start\".\n");
        clearTemp();
        continue;
      }
      else
      {
        addSpaceToTemp();
        addTokenToTemp(token);
        while (token = strtok(NULL, " \n"))
        { // while arguments exist, add them to tempCommand to be added to the history
          addSpaceToTemp();
          addTokenToTemp(token);
        }
        addToHistoryArray();
        start();
      }
    }
    else if (strcmp(token, "background") == 0)
    { // TODO: WIP setup as above
      background();
    }
    else if (strcmp(token, "dalek") == 0)
    {
      addTokenToTemp("dalek");
      token = strtok(NULL, " \n");
      if (token == NULL || (atoi(token) == 0))
      {
        printf("Error: invalid input, expected a number after \"dalek\".\n");
        clearTemp();
        continue;
      }
      else
      {
        addSpaceToTemp();
        addTokenToTemp(token);
        addToHistoryArray();
        int num = atoi(token);
        dalek(num); // Immediately terminate the program with the specific PID
      }
    }
    else if (strcmp(token, "repeat") == 0)
    { // Extra credit function
      repeat();
    }
    else if (strcmp(token, "dalekall") == 0)
    { // Extra credit function
      dalekall();
    }
    else
    {
      printf("Invalid command, please check your input and try again.");
    }

    printf("\n");
  }

  return 0;
}

// Clear tempCommand variable so it can be used in other areas
void clearTemp()
{
  strcpy(tempCommand, "");
}

// Concatenate a space character for use betwen two tokens
void addSpaceToTemp()
{
  addTokenToTemp(" ");
}

// Add some string to the tempCommand variable
void addTokenToTemp(char *str)
{
  strcat(tempCommand, str);
}

// Allocate space and add tempCommand's contents to the history array then clear tempCommand
void addToHistoryArray()
{
  historyArray = (char **)realloc(historyArray, (historyArrayCounter + 1) * sizeof(char *)); // dynamically assign space to the array
  historyArray[historyArrayCounter++] = strdup(tempCommand);                                 // add the entire command to the array
  clearTemp();                                                                               // clear tempCommand variable so it can be used elsewhere
}

void addToParamterArray() {
  parameterArray = (char**)realloc(parameterArray, (parameterCount + 1) * sizeof(char*)); // dynamically assign space to the array
  parameterArray[parameterCount++] = strdup(tempCommand);
}

// Try to load commands from a history.txt file
void getHistory()
{
  fp = fopen("history.txt", "r"); // try to open the file
  char *temp;
  if (fp != NULL)
  { // if file exists
    do
    {
      temp = fgets(tempCommand, 120, fp); // place a line from history.txt into tempCommand
      tempCommand[strcspn(tempCommand, "\n")] = 0;
      if (temp != NULL)
      {
        addToHistoryArray(); // if the line got placed into tempCommand, add to history array
      }
    } while (temp != NULL); // loop through file until EOF
  }
  if (fp != NULL)
  {
    fclose(fp);
  }
}

// Assigned: Derrick
void movetodir(char *token)
{
  DIR *dir = opendir(token);
  if (dir)
  {
    currentdir = dir;
    currentdirString = token;
    printf("currentdir set to %s", token);
    closedir(dir);
  }
  else if (ENOENT == errno)
  {
    printf("Error: specified directory %s does not exist.", token);
  }
  else
  {
    printf("Error: movetodir command returned an unknown error, please try again");
  }
}

// Assigned: Derrick
void whereami()
{
  printf("%s", currentdirString);
}

// Assigned: Derrick
void history(int flag)
{
  if (flag == TRUE)
  { // "-c" Flag encountered, clear history
    int i;
    for (i = historyArrayCounter - 1; i >= 0; i--)
    {
      free(historyArray[i]);
    }
    free(historyArray);
    historyArrayCounter = 0;
    printf("history cleared");
  }
  else
  { // No "-c" flag, print history
    int i;
    if (historyArrayCounter == 0) {  // history is clear, don't access free'd memory
      printf("0: history");
      return;
    }
    for (i = historyArrayCounter - 1; i >= 0; i--)
    {
      if (i != 0)
      {
        printf("%d: %s\n", (historyArrayCounter - i - 1), historyArray[i]);
      }
      else
      {
        printf("%d: %s", (historyArrayCounter - i - 1), historyArray[i]);
      }
    }
  }
}

// Assigned: Derrick
// Note: as the document didn't specify, the behavior of the "history -c" command
//   means those commands do not get saved to the history file
//   (they get physically cleared from the data structure)
void byebye()
{
  fp = fopen("./history.txt", "w+"); // w+ trancates the file to zero length if it exists, otherwise creates the file
  int i;
  for (i = 0; i < historyArrayCounter - 1; i++)
  {
    char *str = strcat(historyArray[i], "\n"); // concatenate a newline character after the given string
    fputs(str, fp);                            // place it in the file
  }
  fclose(fp);
  printf("History saved to history.txt. Have a good day :)\n");
}

// Assigned: James
void replay(int num) {
  char* command = historyArray[historyArrayCounter - num - 2];
  command = strtok(command, " \n");
  
  if (strcmp(command, "movetodir") == 0) {
      addTokenToTemp("movetodir");  // Add first argument to tempCommand
      command = strtok(NULL, " \n");  // Get next argument in the command
      addSpaceToTemp();             // Add a space after the first token "movetodir"
      addTokenToTemp(command);        // Add the argument to tempCommand
      addToHistoryArray();          // All arguments added, tempCommand ready to be added to history array
      movetodir(command);             // "Move" to directory specified by argument saved in token
    }
    else if (strcmp(command, "whereami") == 0) {
      addTokenToTemp("whereami");
      addToHistoryArray();
      whereami(); // Print the value of the currentdirString variable
    }
    else if (strcmp(command, "history") == 0) {
      addTokenToTemp("history");
      command = strtok(NULL, " \n");
      if (strcmp(command,"-c") == 0) {
        addSpaceToTemp();
        addTokenToTemp(command);
        addToHistoryArray();
        history(TRUE);  // '-c' Flag included, clear history list
      }
      else if(command != NULL) {
        printf("Error: unkonwn parameter for command \"history\".");
      }
      else {
        addToHistoryArray();
        history(FALSE); // '-c' Flag not included, print history list
      }
    }
    else if (strcmp(command, "byebye") == 0) {
      addTokenToTemp("byebye");
      addToHistoryArray();
      byebye(); // Save history contents to a file
      exit(0);  // Exit the loop
    }
    else if (strcmp(command, "replay") == 0) {
      addTokenToTemp("replay");
      command = strtok(NULL, " \n");
      if (command == NULL || (atoi(command) == 0)) {
        printf("Error: invalid input, expected a number after \"replay\".\n");
        clearTemp();
        return;
      }
      else {
        addSpaceToTemp();
        addTokenToTemp(command);
        addToHistoryArray();
        int num = atoi(command);
        replay(num); // Re-execute the command labeled with number in the history
      }
    }
    else if (strcmp(command, "start") == 0) { // TODO: WIP setup, must support arbitrary parameters
      addTokenToTemp("start");
      command = strtok(NULL, " \n");
      if (command == NULL) {
        printf("Error: invalid input, expected a path after \"start\".\n");
        clearTemp();
        return;
      }
      else {
        addSpaceToTemp();
        addTokenToTemp(command);
        while (command = strtok(NULL, " \n")) {  // while arguments exist, add them to tempCommand to be added to the history
          addSpaceToTemp();
          addTokenToTemp(command);
        }
        addToHistoryArray();
        start();
      }
    }
    else if (strcmp(command, "background") == 0) {  // TODO: WIP setup as above
      background();
    }
    else if (strcmp(command, "dalek") == 0) { // TODO: Check if argument not included or not a number (invalid input)
      addTokenToTemp("dalek");
      command = strtok(NULL, " \n");
      if (command == NULL || (atoi(command) == 0)) {
        printf("Error: invalid input, expected a number after \"dalek\".\n");
        clearTemp();
        return;
      }
      else {
        addSpaceToTemp();
        addTokenToTemp(command);
        addToHistoryArray();
        int num = atoi(command);
        dalek(num); // Immediately terminate the program with the specific PID
      }
    }
    else if (strcmp(command, "repeat") == 0) {    // Extra credit function
      repeat();
    }
    else if (strcmp(command, "dalekall") == 0) {  // Extra credit function
      dalekall();
    }
    else {
      printf("Invalid command, please check your input and try again.");
    }
}


// Assigned: James
void start() {
  char* command = historyArray[historyArrayCounter - 1];
  command = strtok(command, " ");
  command = strtok(NULL, " "); // iterate once to get the pathname
  char* pathname = command;
  command = strtok(NULL, " "); // loop through the history of the command to get to the first paratmeter if there is any
  addToParamterArray();
  parameterArray[0] = pathname;
  for(int i=1; (i < MAX_COMMAND_ARGS - 1) && (command != NULL); i++) {
    addToParamterArray();                   // For every parameter, realloc and store.
    parameterArray[i] = command;
    command = strtok(NULL, " ");
  }
  pid_t pid=fork();
  if (pid==0) { //if it is the child process 
    execv(pathname,parameterArray); // execute
  }
  else { // if it isn't the child process
    waitpid(pid,0,0); // wait for child to exit before continuing
  }
  free(parameterArray);
  parameterCount = 0;
  
}

// Assigned: James
void background() {
  char* command = historyArray[historyArrayCounter - 1];
  command = strtok(command, " ");
  command = strtok(NULL, " "); // iterate once to get the pathname
  char* pathname = command;
  command = strtok(NULL, " "); // loop through the history of the command to get to the first paratmeter if there is any
  addToParamterArray();
  parameterArray[0] = pathname;
  for(int i=1; (i < MAX_COMMAND_ARGS - 1) && (command != NULL); i++) {
    addToParamterArray();                   // For every parameter, realloc and store.
    parameterArray[i] = command;
    command = strtok(NULL, " ");
  }
  pid_t pid=fork();
  if (pid==0) { //if it is the child process 
    printf("pid : %d", getpid());   // print the newly generated process id
    execv(pathname,parameterArray); // execute
  }
  free(parameterArray);
  parameterCount = 0;
}

// Assigned: James
void dalek(int num)
{
  int result = kill(num, SIGKILL);
  if (result == 0)
  {
    printf("\nPID: %d was successfully terminated.", num);
  }
  else
  {
    printf("\nSignal Failed. Error Number: %d", errno);
  }
}

// Extra Credit function
void repeat()
{
  printf("repeat");
}

// Extra Credit function
void dalekall()
{
  printf("dalekall");
}

