#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "shellfunc.h"
#include "history.h"
#include "alias.h"

// Uses getenv function so we can initialise variables for the path and home directory
char* loadEnvironment () {
  printf("Loading environment...\n");
  
  char* path = getenv("PATH");
  char* home = getenv("HOME");

  
  
  chdir(home); // Changes directory to the home directory
  
  loadHistory();
  
  return path;
}

// Parses user input and tokenises it
char** parseInput(char buffer[]) {
  char delims[] = " \t|><&;";  // Delimiters for parsing
  int i = 0;

  char** token = malloc (sizeof(char*)*50);
  token[i] = strtok(buffer,delims); // Tokenised input stored in array, each index starts after delimiter

  while(token[i] != NULL){
    i++;
    token[i] = strtok(NULL, delims); // Append NULL to end of token array so we have clear end point
  }

/* DO NOT DELETE - handles seg fault and stops everything from breaking C: <- make sure to remove do not delete comment before project submission :p */
  if (token[0] == NULL) {
    token[0] = "";
  }
  
  return token;
}

// Executes command; passed in as parameter
void execute (char* args[]) {
  pid_t p = fork(); // gets the id of child process, parent process > 0, child will always be 0, error is -1

  if (p > 0) { // Checks for parent process
    wait(NULL);
  } else if (p == 0) { // Checks for child process
    execvp(args[0], args);
    perror(args[0]);
    exit(1);
  } else { // Error
    printf("Something went wrong\n");
  }
}

// Executes built in command; passed in as parameter
void executeBuiltIn (char* args[]) {
  char* cmds[] = { "cd", "getpath", "setpath", "history", "alias", "unalias" }; // Array containing built in commands
  
  void (*builtIns[])(char**) = { cd, getPath, setPath, listHistory, alias, unalias }; // Array containing pointers to built-ins
  
  for (int i = 0; i < 6; i++) {
    if (strcmp(args[0], cmds[i]) == 0) {
      builtIns[i](args);
    }
  }
}  


// Prints whats stored in PATH
void getPath (char* args[]) {
  if (args[1]) {
    printf("Error: Too many parameters, none are required\n");
  } else {
    printf("%s\n", getenv("PATH"));
  }
}

// Overwrites what stored in PATH with users input
void setPath (char* args[]) {
  if (args[1] == NULL) {
    printf("Error: No parameter provided, please enter a path\n");
  } else if (args[2]) {
    printf("Error: Too many parameters, please only enter one\n");
  } else {
    setenv("PATH", args[1], 1); 
  }
}

// cd command, changes working directory
void cd (char* args[]) { 
  char* home = getenv("HOME"); // User's home path
  if (args[1] == NULL) chdir(home); // No argument case, cd to home path
  else if (args[2]) printf ("Too many arguments \n");
  else if (chdir(args[1]) != 0) perror(args[1]); // Descriptive error prints using perror 
}

// Calculates history index <<<< not quite sure how to comment this function and it's contents


// Exits shell
void exitShell (char* path) {
  setenv("PATH", path, 1);
  printf("%s\n", getenv("PATH")); 
  saveHistory();
  printf("exiting...\n");
  exit(0);
}

