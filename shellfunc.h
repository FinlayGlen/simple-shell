char* loadEnvironment ();

char** parseInput(char buffer[]);

void execute (char* args[]);

void executeBuiltIn (char* args[]);

void getPath (char* args[]);

void setPath (char* args[]);

void cd (char* args[]);

void exitShell (char* path);
