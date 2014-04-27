#define size 1024

void addHistory(char*);
void showHistory(int);
void recall(int);
void showArgs(char**, int);

char* args[size];
char* history[size];
int historyIndex;

