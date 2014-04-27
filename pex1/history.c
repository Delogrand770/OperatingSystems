#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

int historyIndex = 0;
void addHistory(char* cmd){
	//Prevent duplicate history adds
	if (historyIndex > 0){
		if (strcmp(cmd, history[historyIndex - 1]) == 0){
			return;
		}
	}

	if (historyIndex < size){ //Add new history item
		history[historyIndex] = strdup(cmd);
		historyIndex++;
	}else{ //History is full so shift then add new item
		int i;
		for(i = 0; i < size; i++){
			history[i] = history[i + 1];
		}
		history[size - 1] = strdup(cmd);
	}
}

void showHistory(int num){
	//Prevent out of bounds history requests
	if (num > historyIndex || num < 0){
		num = historyIndex;
	}
	//Show history items
	int i;
	for(i = historyIndex - num; i < size; i++){
		if (history[i]){
			printf("%d %s\n", historyIndex - i, history[i]);
		}
	}
}

void recall(int num){
	if (num >= 0 && num < size){
		if (history[historyIndex - num - 1] != NULL){
			printf("%d %s\n", num+1, history[historyIndex - num - 1]);
		}else{
			printf("There is no history data at slot %d\n", num + 1);
		}
	}else{
		printf("Invalid input. History recall is from 1 to %d\n", size);
	}
}

void showArgs(char** args, int argc){
	printf("args:\n");
	int i;
	for (i=0; i < argc; i++){
		printf("\t%s\n", args[i]);
	}
}
