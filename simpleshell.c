#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shellfunc.h"
#include "history.h"
#include "alias.h"

//LOL, erm ok, was playing around with persistent history and aliases, if "exit"
//is a command in the history, it will be executed...
//will fix in the lab, its too late right now.

//alias cant be something like "alias hi echo hello"
//even if you surround parameters with quotes,
//still gives error "too many arguments"

int main (void) {
  char buffer[512];
  char* output = "";
  char* builtIn[6] = {"cd", "getpath",  "setpath",  "alias",  "unalias",  "history"};
  char** parsed = NULL;
  int isExit = 0;
  
  char* path = loadEnvironment();

  do {
    // Display prompt
    printf("> ");
    
    // Read + Parse input
    output = fgets(buffer, sizeof(buffer), stdin);
    
    if (!output) {
      printf("\n");
      break;
    }

    buffer[strcspn(buffer, "\n")] = 0;
    parsed = parseInput(buffer);

    if (strcmp(parsed[0], "") == 0) {
	continue;
    }

    //check for history invocation
    if (strcspn("!", parsed[0]) == 0) {
      char* historyCmd = invokeHistory(parsed);
      if (strcmp(historyCmd, "") != 0) {
	strcpy(buffer, historyCmd);
	parsed = parseInput(buffer); //reparse history command
      } 
    } else {
      addHistory(parsed);
    }
    
    //check for aliased command
    char* aliasCmd = parsed[0];
    if (strcmp(parsed[0], "unalias") != 0 && strcmp(parsed[0], "alias") != 0) {
      aliasCmd = invokeAlias(parsed);
    }
    if (aliasCmd != parsed[0]) {
      strcpy(buffer, aliasCmd);
      parsed = parseInput(buffer); //reparse alias command
    }
    
    if (strcmp(parsed[0], "exit") == 0 && parsed[1] != NULL) {
      printf("exit: No parameters are allowed!\n");
    } else if (strcmp(parsed[0], "exit") == 0) {
      isExit = 1;
    }

    for (int i = 0; i < 6; i++) {
      if (strcmp(builtIn[i], parsed[0]) == 0 || !output || strcmp(parsed[0], "exit") == 0) {
	executeBuiltIn(parsed);
	break;
      } else {
	execute(parsed);
	break;
      }
    }
    
    // memory management
    if (parsed != NULL) { 
      free(parsed);
    } else if (aliasCmd != NULL) {
      free(aliasCmd);
    }
    
    parsed = NULL;
    aliasCmd = NULL;
  } while (isExit == 0); // exits while loop if buffer == exit
  
  // cleanly exit shell
  exitShell(path);

  return 0;
}
