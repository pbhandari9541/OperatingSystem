//By Valentin Gazeau
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_LINE 80
#define TOK_DELIM " \t\r\n\a!"

char ** parse(char * command);
int execute( char ** args);

int main(){

	char *command;
	char **arguments;
	char *history[30];
	int cmdcount = 0, i = 0, tmp = 0, stat = 1, count = 0;
	ssize_t bufsize = 0;

	do{
	start:
		printf("$valsh> ");
		getline(&command, &bufsize, stdin);

		if(strncmp("exit", command, 4) == 0){
			stat = 0;
			return 1;

		}else if(strncmp("history", command, 7) == 0){
			count = 0;
			for( i = tmp; count<10 && i>0; i--){
				printf("%i. %s", i, history[i-1]);
				count++;
			}
		}else{

			if(strncmp("!!", command, 2) == 0){
				if( tmp == 0){
					printf("Error: No commands in history\n");
					goto start;
				}else{
				command = strdup(history[tmp-1]);
				printf("Running : %s", command);
				}

			}else{ 
				if(strncmp("!", command, 1) == 0){
					arguments = parse(command);
					cmdcount = atoi(arguments[0]);
					
					if(cmdcount <= 0 || cmdcount > tmp){
						printf("Error: No such command\n");
						goto start;	
					}else{
						command = strdup(history[cmdcount-1]);
						printf("Running : %s", command);
					}
				}

			}
			history[tmp] = strdup(command);
			tmp++;
			arguments = parse(command);

			if(strncmp("cd", command, 2) == 0){
				chdir(arguments[1]);
			}else{
				stat = execute(arguments);
			}
		}

	} while (stat);

	free(command);
	free(arguments);
	return 0;
}

//parsing the command for arguments
char ** parse(char * command){
	int buf = MAX_LINE, pos = 0;
	char **tokens = malloc(buf * sizeof(char*));
	char *token;

	token = strtok(command, TOK_DELIM);
	while(token != NULL){
		tokens[pos] = token;
		token = strtok(NULL, TOK_DELIM);
		pos++;
	}
	tokens[pos] = NULL;
	return tokens;
}

//executing the command
int execute( char ** args){
	pid_t pid, wait;
	int stat;

	pid = fork();

	if(pid == 0){
		if(execvp(args[0], args) == -1){
			printf("Could not execute: %s\n",args[0]);	
		}
		exit(EXIT_FAILURE);
	}else if( pid < 0){
		printf("Error forking\n");
		exit(EXIT_FAILURE);
	}else{
		do{
			wait = waitpid(pid, &stat, WUNTRACED);		
		}while(!WIFEXITED(stat) && ! WIFSIGNALED(stat));
	}
	return 1;
}
