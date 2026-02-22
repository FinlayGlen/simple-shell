#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "history.h"

char history[20][512];
int saved = 0;
int isLooped = 0;

void loadHistory () {

}

void saveHistory () {

}

void addHistory(char** parsed) {
  int i = 0;
  char cmd[512] = {""};
  while (parsed[i] != NULL) {
    strcat(cmd, parsed[i]);
    strcat(cmd, " ");
    i++;
  }
  
  if (strcmp(cmd, " ") != 0) {
    strcpy(history[saved], cmd);
  }
  saved = (saved + 1) % 20;
  // printf("saved: %d\n", saved); COMMENTED OUT FOR ALIASES
  if (saved == 0){
    isLooped = 1;
  }
  //printf("%d\n", isLooped); COMMENTED OUT FOR ALIASES
}

int calculateIndex(char* arg, int length) {
  int n = 0;
  if (strcmp(arg, "!!") == 0) {
    n = saved;
  } else if (length == 2 || (length == 3 && arg[1] == '-')) {
    n = (arg[length-1] - '0');
  } else if (length == 3 || length == 4){
    n = (arg[length-2] - '0') * 10 + (arg[length-1] - '0');
  } else {
    return 0;
  }

  if (arg[1] == '-') {
    n = 20 - n;
  }

  n = n -1;
  
  if (strcmp(history[4], "") != 0) {
    return (n+saved)%20;
    
  }
  
  return n;
}

char* invokeHistory (char* args[]) {
  int n = 0;
    
  n = calculateIndex(args[0], strlen(args[0]));

  printf("n: %d\n", n);
  
  if (n < 0 ||  n > 19) {
    printf("Error: History must be from -19 to 20\n");
  } else if (strcmp(history[n], "") == 0) {
    printf("Error: No history for that index\n");
  } else {
    return history[n];
  }
  
  return "";
}

void listHistory (char* args[]) {
  if (strcmp(history[19], "") == 0) {
    for (int i = 0; i < 20; i++) {
      printf("%d: %s\n", i+1, history[i]);
    }
  } else {
    for (int i = saved; i < 20+saved; i++) {
      printf("%d: %s\n", i-saved+1, history[i%20]);
    }
  }
}
