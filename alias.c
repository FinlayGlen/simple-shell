#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alias.h"

char* aliasMap[2][10] = { {"k","j","","","","","","","",""}, {"cd","ls","","","","","","","",""}};

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
    printf("Error: Not enough parameters provided please use format 'alias <name> <command>'\n");
    return;
  }
  else if (args[3]) {
    printf("Error: Too many parameters, please use format 'alias <name> <command>'\n");
    return;
  }
  else if (strstr(args[1], "!")){
    printf("Error: Please avoid using '!' in aliases");
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
    printf("Error: No more aliases can be stored:\n Remove aliases using 'unalias <name>' to free slots\n");
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
  aliasMap[0][i] = malloc(strlen(args[1]));
  strcpy(aliasMap[0][i], args[1]);
  aliasMap[1][i] = malloc(strlen(args[2]));
  strcpy(aliasMap[1][i], args[2]);
  return;
}


void unalias (char* args[]){

  if(args[1] == NULL){
    printf("Not enough parameters provided, please use format 'unalias <name>'\n");
    return;
  }
  else if (args[3]) {
    printf("Error: Too many parameters, please use format 'unalias <name>'\n");
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

//strcpy 
  char* invokeAlias(char* args[]){
    for(int i = 0; i < 10; i++){
      if (!strcmp(aliasMap[1][i], args[0])){
        char* outputCmd = "";
        strcpy(outputCmd, aliasMap[1][i]);
        strcat(outputCmd, args[1]);
        strcat(outputCmd, args[2]);

        return (outputCmd) ;
      }
    }
    return args[0];
  }