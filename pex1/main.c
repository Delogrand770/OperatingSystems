/*
Documentation:
C2C Roman Young explained how to use the strtok command.
He also told me to use strncmp instead of strcmp to get my string comparisons to work.

C2C Alex Thompson told me about the strdup command to get my history methods to work.

The make file is based off the example done in class and the fork/execvp was done in EI.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

void getInput();
void lookupInput();

int count;
int keepAlive = 1;

int main(int argc, char* argv[]){
	int historyIndex = 0;
	printf("Welcome to the Fall 2012 CS483 shell assignment.\n");
	while(keepAlive){
		getInput();
		lookupInput(&historyIndex);
		//showArgs(args, count);
	}
	return EXIT_SUCCESS;	
}

void getInput(){
	printf("Prompt> ");
	char userInput[size];
	char* token;
	count = 1;
	if (fgets(userInput, size, stdin)){
		//Convert input into array
		token = (char*) strtok(userInput, " \t\n");
		args[0] = (char*) token;
		while((token = (char*) strtok(NULL, " \t\n")) != NULL){
			args[count] = token;
			count++;
		}
		args[count] = NULL;
		
		if (args[0] != NULL){	
			addHistory(userInput);
		}
	}
}

void lookupInput(){
	//Catch empty commands
	if (args[0] == NULL){
		return;
	}

	//Determine which command was executed
	if (strncmp(args[0], "exit", 4) == 0){
		keepAlive = 0;
	}else if (strncmp(args[0], "history", 7) == 0){
		if (count == 2 && atoi(args[1]) > 0){
			showHistory(atoi(args[1]));
		}else{
			showHistory(10);
		}
	}else if (strncmp(args[0], "recall", 6) == 0){
		recall(atoi(args[1]) - 1);
	}else{
		int pid = fork();
		if (!pid){
			execvp(args[0], args);
			//error handling if exec doesnt work
			printf("Please enter a valid command.\n");
			exit(1);
		}else{
			wait(NULL);
			//sleep(2);
		}
	}
}

