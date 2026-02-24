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
  FILE *fptr;
  fptr = fopen(".hist_list", "r"); // Open the file
  
  if(fptr == NULL) { // Check to handle file not existing yet
    printf("Not able to open the file.");
    return;
  }
  
  char fileArr[1000]; // This is where the current line of file being read is stored
  int i = 0;
  while(fgets(fileArr, 1000, fptr)) {
    fileArr[strcspn(fileArr, "\n")] = 0; // Removes the newline to match array format
    strcpy(history[i], fileArr); // Copies current line of file into history array
    i++;
  }
  fclose(fptr); // Close the file 
}

void saveHistory () {
  FILE *fptr;
  fptr = fopen(".hist_list", "w"); // Open the file
  
  if (strcmp(history[19], "") == 0) { // If last index of history is empty
    for (int i = 0; i < 20; i++) {
      if (strcmp(history[i], "") != 0) { // If current index in history isnt empty
	fprintf(fptr, "%s\n", history[i]); // Store current index in the file
      }
    }
  } else {
    for (int i = saved; i < 20+saved; i++) {
      fprintf(fptr, "%s\n", history[i%20]); // Circular case for when i > 20
    }
  }
  fclose(fptr); // Close the file
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
  if (strcmp(arg, "!!") == 0) {
    n = saved;
  } else if (length == 2 || (length == 3 && arg[1] == '-')) { // Single digit or negative single digit case
    n = (arg[length-1] - '0');
  } else if (length == 3 || length == 4){ // Double digit or negative double digit case
    n = (arg[length-2] - '0') * 10 + (arg[length-1] - '0');
  } else {
    return 0;
  }

  if (arg[1] == '-') {
    n = 20 - n; // Calculates index of negative history invocation
  }

  n = n -1; // Calculates index of positive history invocation
  
  if (strcmp(history[19], "") != 0) {
    return (n+saved)%20;
  }
  
  return n;
}

char* invokeHistory (char* args[]) {
  int n = 0;
    
  n = calculateIndex(args[0], strlen(args[0])); 
  
  if (n < 0 ||  n > 19) {
    printf("Error: History must be from -19 to 20\n");
  } else if (strcmp(history[n], "") == 0) {
    printf("Error: No history for that index\n");
  } else {
    return history[n];
  }
  
  return "";
}

void listHistory (char* args[]) { // If last index of history is empty
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
