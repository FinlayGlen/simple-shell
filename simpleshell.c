#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shellfunc.h"
#include "history.h"


int main (void) {
  char buffer[512];
  char* output = "";
  char* builtIn = "cd getpath setpath alias unalias history";
  char** parsed;
  
  char* path = loadEnvironment();
  
  do {
    // Display prompt
    printf("> ");
    
    // Read + Parse input
    output = fgets(buffer, sizeof(buffer), stdin);
    
    if (output == NULL) {
      break;
    }
    
    buffer[strcspn(buffer, "\n")] = 0;

    parsed = parseInput(buffer);
    
    if (strcspn("!", parsed[0]) == 0) {
      strcpy(buffer, invokeHistory(parsed));
      parsed = parseInput(buffer);
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
  
  
  // cleanly exit shell
  exitShell(path);

  return 0;
}
