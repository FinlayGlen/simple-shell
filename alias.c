#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alias.h"

char* aliasMap[2][10] = { {"","","","","","","","","",""}, {"","","","","","","","","",""} };

void alias (char* args[]){

  // Print aliases when no parameters
  if (args[1] == NULL){

    //Check if there are any aliases
    int i = 0;
    while((!strcmp(aliasMap[0][i], ""))){
      if(i > 9){
        printf("There are no aliases\n");
        return;
      } 
      i++;
    }
    //Otherwise print aliases
      for(int x = 0; x < 10; x++){
        printf("'%s' '%s'\n", aliasMap[0][x],aliasMap[1][x]);
      }
    return;
  }

  // Validation
  else if(args[2] == NULL){
    printf("alias: Not enough arguments provided please use format 'alias <name> <command>'\n");
    return;
  }
  else if (strstr(args[1], "!")){
    printf("alias: Please avoid using '!' in aliases");
      return;
  }


  // Setting alias
  int i = 0;
  // Find a free slot and check if the alias is in use 
  while(( (strcmp(aliasMap[0][i], "")) && ((strcmp(aliasMap[0][i], args[1]))) ) ){
    if(i == 9){
      break;
    }
    i++;
  }

  if((i == 9) && (strcmp(aliasMap[0][i],""))){
    printf("alias: No more aliases can be stored:\n Remove aliases using 'unalias <name>' to free slots\n");
    return;
  }

    // If alias is in use
    if(!(strcmp(aliasMap[0][i], args[1]))){

      free(aliasMap[0][i]);
      aliasMap[0][i] = "";
      free(aliasMap[1][i]);
      aliasMap[1][i] = "";
      
      printf("Alias has been replaced\n");
    }

  // Allocate memory and add to array
  //fix: for the mallocs, + 1 to account for null terminator
  aliasMap[0][i] = malloc(strlen(args[1]) + 1);
  strcpy(aliasMap[0][i], args[1]);
  aliasMap[1][i] = malloc(strlen(args[2]) + 1);

  int j = 2;
  char cmd[512] = {""};
  while (args[j] != NULL) {
    strcat(cmd, args[j]);
    strcat(cmd, " ");
    j++;
  }
  strcpy(aliasMap[1][i], cmd);

  return;
}


void unalias (char* args[]){

  if(args[1] == NULL){
    printf("alias: Not enough arguments provided, please use format 'unalias <name>'\n");
    return;
  }
  else if (args[3]) {
    printf("alias: Too many arguments, please use format 'unalias <name>'\n");
    return;
  }

  else{   
    int i = 0;
    // search for alias
    while(strcmp(aliasMap[0][i], args[1])){
      if((i == 9) && (strcmp(aliasMap[0][i], args[1]))){
        printf("Alias %s does not exist\n", args[1]);
        return;
      }
      i++;
    }
    // free memory & remove array elements 
      free(aliasMap[0][i]);
      aliasMap[0][i] = "";
      free(aliasMap[1][i]);
      aliasMap[1][i] = "";
  }  
}

char* invokeAlias(char* args[]){
  char* cmd = malloc(sizeof(char)*512);

  for(int i = 0; i < 10; i++){
    if (strcmp(aliasMap[0][i], args[0]) == 0){
      strcpy(cmd, aliasMap[1][i]);
      int j = 1;
      while (args[j] != NULL) {
	strcat(cmd, " ");
	strcat(cmd, args[j]);
	j++;
      }
      return cmd;
    }
  }
  free(cmd);
  cmd = NULL;
  return args[0];
}


void saveAlias() {
  FILE *f = fopen(".aliases", "w");
  if (f == NULL) return;

  for (int i = 0; i < 10; i++) {
    //only save slots that arent empty
    if (strcmp(aliasMap[0][i], "") != 0) {
      fprintf(f, "%s %s\n", aliasMap[0][i], aliasMap[1][i]);
    }
  }
  fclose(f);
  freeAliases();
}


void loadAlias() {
  FILE *f = fopen(".aliases", "r");
  if (f == NULL) return;

  char buffer[512]; //it is assumed that the line in the terminal is 512 characters long
  int i = 0;

  //read in each line until eof or max aliases reached
  while (fgets(buffer, sizeof(buffer), f) && i < 10) {
    buffer[strcspn(buffer, "\n")] = 0; //remove newline character

    //split line into alias and command
    char *alias = strtok(buffer, " ");
    char *command = strtok(NULL, "");

    if (alias != NULL && command != NULL) {
      aliasMap[0][i] = malloc(strlen(alias) + 1);
      strcpy(aliasMap[0][i], alias);
      aliasMap[1][i] = malloc(strlen(command) + 1);
      strcpy(aliasMap[1][i], command);
      i++;
    }
  }
  fclose(f);
}


void freeAliases () {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 10; j++) {
      if (strcmp(aliasMap[i][j], "") != 0) {
	free(aliasMap[i][j]);
	aliasMap[i][j] = NULL;
      }
    }
  }
}
