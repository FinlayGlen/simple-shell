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
  char* builtIn = "cd getpath setpath alias unalias history";
  char** parsed;
  
  char* path = loadEnvironment();
  //load persistent history and aliases on startup
  loadHistory();
  loadAlias();

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
    
    

      //check for aliased command
      char* aliasCmd = invokeAlias(parsed);
      if (aliasCmd != parsed[0]) {
        strcpy(buffer, aliasCmd);
        parsed = parseInput(buffer); //reparse alias command
      }

      if (strcspn("!", parsed[0]) == 0) {
	//check for history invocation
	  char* historyCmd = invokeHistory(parsed);
	  if (strcmp(historyCmd, "") != 0) {
	    strcpy(buffer, historyCmd);
	    parsed = parseInput(buffer); //reparse history command
	  } 
      } else {
          addHistory(parsed);
	}

    
    if (strstr(builtIn, parsed[0]) || !output || strcmp(parsed[0], "exit") == 0) {
      executeBuiltIn(parsed);
    } else {
      execute(parsed);
    }    
  } while (strcmp(parsed[0], "exit") != 0); // exits while loop if output is NULL or buffer == exit

  // memory management
  free(parsed);
  parsed = NULL;

  saveHistory();
  saveAlias();
  
  // cleanly exit shell
  exitShell(path);

  return 0;
}
