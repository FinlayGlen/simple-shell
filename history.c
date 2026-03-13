#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "history.h"

char history[20][512];
int saved = 0;

void loadHistory () {
  
  FILE *f = fopen(".hist_list", "r");
  if (f == NULL) return;

  char buffer[512];
  while (fgets(buffer, sizeof(buffer), f)) {
    buffer[strcspn(buffer, "\n")] = 0;

    strcpy(history[saved], buffer); //add history to buffer
    saved = (saved + 1) % 20; //circular buffer
  }
  fclose(f);;
}

void saveHistory () {
  chdir("/HOME");
  FILE *f = fopen(".hist_list", "w");
  if (f == NULL) return;

  for (int i = 0; i < 20; i++) {
    if (strcmp(history[(saved + i) % 20], "") != 0) {
      fprintf(f, "%s\n", history[(saved + i) % 20]);
    }
  }
  
  fclose(f);
}

void addHistory(char** parsed) {
  int i = 0;
  char cmd[512] = {""};
  while (parsed[i] != NULL) { // Concatenates tokensied command back into full command for history
    strcat(cmd, parsed[i]);
    strcat(cmd, " ");
    i++; 
  }
  
  if (strcmp(cmd, " ") != 0 && strcmp(cmd, "exit ") != 0) { // If cmd isnt empty and isnt exit
    strcpy(history[saved], cmd); // Save cmd in history at array saved
    saved = (saved + 1) % 20; // Iterate saved circularly
  }
}

int calculateIndex(char* arg, int length) {
  int n = 0;
  if ((length > 3 && (length > 4 && arg[1] != '-')) || (length > 4 && arg[1] == '-')) {
    return -1;
  }
  
  if (strcmp(arg, "!!") == 0) {
    if (strcmp(history[19], "") == 0) {
      n = saved;
    } else {
      n = 20;
    }
  } else if (length == 2 || (length == 3 && arg[1] == '-')) { // Single digit or negative single digit case
    n = (arg[length-1] - '0');
  } else if (length == 3 || length == 4){ // Double digit or negative double digit case
    n = (arg[length-2] - '0') * 10 + (arg[length-1] - '0');
  } else {
    return 0;
  }

  if (arg[1] == '-' && arg[2] - '0' != 0) {
    n = 21 - n; // Calculates index of negative history invocation
  }
  
  return n-1;
}

char* invokeHistory (char* args[]) {
  int n = 0;
    
  n = calculateIndex(args[0], strlen(args[0])); 

  if (args[1] != NULL) {
    printf("history: too many arguments, none are needed\n");
    return " ";
  }
  
  if (n < 1 ||  n > 19) {
    printf("history: History must be from -19 to 20\n");
  } else if (strcmp(history[n], "") == 0) {
    printf("history: No history for that index\n");
  } else if (strcmp(history[19], "") == 0) {
    return history[n];
  } else {
    return history[(n+saved)%20];
  }
  
  return " ";
}

void listHistory (char* args[]) { // If last index of history is empty
  if (args[1] != NULL) {
    printf("history: too many arguments, none are needed\n");
    return;
  }
  
  if (strcmp(history[19], "") == 0) {
    for (int i = 0; i < 20; i++) {
      if (strcmp(history[i], "") != 0) { // If current index of history isnt empty
	printf("%d: %s\n", i+1, history[i]); // List current index of history
      }
    }
  } else {
    for (int i = saved; i < 20+saved; i++) {
      printf("%d: %s\n", i-saved+1, history[i%20]); // Circular case for when i > 20
    }
  }
}
