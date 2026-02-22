#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alias.h"

char* aliasMap[2][10] = { {"","","","","","","","","",""}, {"","","","","","","","","",""}};

void alias (char* args[]){

  // Print all aliases when just 'alias' is written
  if (args[1] == NULL){

    //Check if there are any aliases
    int i = 0;
    while(!strcmp(aliasMap[0][i], "")){
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
  else if (strcmp(aliasMap[0][9], "")){
    printf("Error you have reached the maximum amount of saved aliases\n");
    return;
  }
  else if(args[2] == NULL){
    printf("Error: Not enough parameters provided please use format 'alias <name> <command>'\n");
    return;
  }
  else if (args[3]) {
    printf("Error: Too many parameters, please use format 'alias <name> <command>'\n");
    return;
  }


  //Setting alias
  else{   
    int i = 0;
    // find a free slot
    while(strcmp(aliasMap[0][i], "")){
      i++;
    }
    // setting memory for 
      aliasMap[0][i] = malloc(strlen(args[1]));
      strcpy(aliasMap[0][i], args[1]);
      aliasMap[1][i] = malloc(strlen(args[2]));
      strcpy(aliasMap[1][i], args[2]);
  }
  return;
}


// Remember to free memory malloced in alias
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
    // search for the alias
    while(strcmp(aliasMap[0][i], args[1])){
      i++;
    }
    // removing from memory & array 
      free(aliasMap[0][i]);
      aliasMap[0][i] = "";

      free(aliasMap[1][i]);
      aliasMap[1][i] = "";

  }  
  

}