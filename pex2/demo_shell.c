#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Documentation:
// I used the website provided to understand how to work pipes.
// I also used the website http://stackoverflow.com/questions/1461331/writing-my-own-shell-stuck-on-pipes
// to debug my code that was not working from the first website.

void pipeCommands(char**, char**);//Pipes two commands created from a user inputted piped command
char** tokenize(char* str);//returns an array of tokens.  array is null terminated
int checkForPipe(char**);
char** makeCMD(char**, int);

int main (int argc, char* argsv[]){
	char cwd[1024]; //My Change
	printf("\nWelcome to the Fall 2012 CS483 demo shell.\n");
	char input[128];
	list* history = new_list();
	while(1){
		//reads a command line
		do {
			//print a prompt	
			getcwd(cwd, sizeof(cwd)); //My Change	
			printf("%s$ ", cwd); //My Change
			gets(input);
		} while (strlen(input)==0);
		char* command = malloc(sizeof(char) * (strlen(input) + 1));
		strcpy(command, input);
		insert(history, command);
		//printf("'%s'\n",input);
		//parses the command line into tokens
		char** tokens = tokenize(input);
		
		//compare to built in commands
		int cont = 0;		
		while(strcmp(tokens[0], "recall") == 0){
			//process recall
			//error check right number of arguments			
			if(tokens[1] == NULL || tokens[2] != NULL){
				printf("Error using recall command.\nSpecify command number to execute.\n");
				cont = 1;
				break;
			}
			//error check called with number as second argument
			int rcl_cmd = atoi(tokens[1]) + 1;
			if(rcl_cmd < 2){
				printf("Error using recall command.\nSpecify command number to execute.\n");
				cont = 1;
				break;
			}
			if(rcl_cmd>history->size){
				printf("Error using recall command.\nCommand history doesn't go that far back.\nSpecify a vaild command number to execute.\n");
				cont = 1;
				break;
			}
			strcpy(input, get(history, rcl_cmd));
			printf("%s\n", input);
			free(tokens);
			tokens=tokenize(input);				
		}
		if(cont){
			continue;
		}
		if(strcmp(tokens[0], "exit") == 0){
			printf("Terminating Fall 2012 CS483 demo shell.\n\n");			
			return 0;
		}
		if(strcmp(tokens[0], "history") == 0){
			//execute history command
				//get number of history
				int how_many = 10;
				if(tokens[1] != NULL){
					how_many = atoi(tokens[1]);
					if(how_many == 0 || tokens[2] != NULL){
						printf("Error using history command.\nSpecify number of commands to print or leave blank for default (10).\n");
						continue;
					}
				}
				print_list(history, how_many);
				continue;
				//printf("how many: %d\n",how_many);
		}


		//My Changes for cd
		if(strcmp(tokens[0], "cd") == 0){
			if (strncmp(tokens[1], "~", 1) == 0){
				tokens[1]++;
				tokens[1] = malloc(sizeof(char) * (strlen(getenv("HOME"))) + sizeof(char) * (strlen(tokens[1])) + 1);
				tokens[1] = strcat(getenv ("HOME"), tokens[1]);
			}
			chdir(tokens[1]);
			continue;
		}

		if (checkForPipe(tokens)){
			char** cmd1 = makeCMD(tokens, 0);
			char** cmd2 = makeCMD(tokens, 1);
			int fds[2]; // file descriptors
			pipe(fds);
			if (fork() == 0) { //child 1
				dup2(fds[1], 1);
				close(fds[0]);
				close(fds[1]);
				execvp(cmd1[0], cmd1); //execute command 1
				printf("The program '%s' cannot be found\n", cmd1[0]);
				return 1;
			}
			if (fork()  == 0) { //child 2
				dup2(fds[0], 0);
				close(fds[0]);
				close(fds[1]);
				execvp(cmd2[0], cmd2); //execute command 2
				printf("The program '%s' cannot be found\n", cmd2[0]);
				return 1;
			}
			close(fds[0]);
			close(fds[1]);
			//wait for both children to close
			wait(NULL);
			wait(NULL);
			printf("\n");
		}else{
			//fork a child
			int childs_exit_status;
			if(fork() != 0){
				//parent waits for child to terminate
				waitpid(-1, &childs_exit_status, 0);
				//printf("Child returned with status %d\n",childs_exit_status);
			} else {
				//child loads and executes the command
				execvp(tokens[0], tokens); 
				//if we get here, it means execvp didn't work.
				printf("The program '%s' cannot be found\n", tokens[0]);
				return 1;
			}
		}
		free(tokens);//only deallocae the pointer array.  don't deallocate teh strings.  they're actually parts of the input array, which is statically delcared.
	
	}
	return 1;
}

int checkForPipe(char** tokens){
	int i = 0;
	while(tokens[i] != NULL){
		if (strcmp(tokens[i], "|") == 0){
			return 1;
		}
		i++;
	}

}

char** makeCMD(char** tokens, int half){
	int i = 0;
	while(tokens[i] != NULL){
		//printf("'%s'\n", tokens[i]);
		if (strcmp(tokens[i],"|") == 0){	
			//printf("%d", i);
			break;
		}
		i++;
	}
	//printf("%d\n",i);
	char** rtn = malloc( sizeof(char*) * (i + 1) );
	if (!half){
		int j;
		for(j = 0; j < i; j++){
			rtn[j] = tokens[j];
		}
		rtn[j] = NULL;
	}else{
		int j = 0;
		i++;
		while(tokens[i + j] != NULL){
			rtn[j]=tokens[j + i];
			j++;
		}
		rtn[j] = NULL;
	}
	return rtn;
}

char** tokenize(char* str){
	int i = 0;
	const char* delims = " \t\n";
	char* ptr = strtok(str, delims);
	if(ptr){
		i = 1;
	}
	while( strtok(NULL, delims) ){
		i++;
	}
	//printf("num of tokens: %d\n",i);
	char** rtn = malloc( sizeof(char*) * (i + 1) );
	int j;
	for(j = 0; j < i; j++){
		rtn[j] = str;
		while( (*str) != '\0' ){
			str++;
		}
		str++;
		while( isspace(*str) ){
			str++;
		}
	}
	rtn[j] = NULL;
	return rtn;

}
