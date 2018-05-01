/********************* R E V I S I O N   H I S T O R Y ********************

 **************************************************************************/
//gcc alias.c
// ./a.out
// alias dog ls
//alias abc ps
//alias qwe pwd
/* ls ps clear pwd */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CHAR_LIMIT 512
#define TOKEN_LIMIT 50
#define ALIAS_LIMIT 10
#define INTERNAL_COMMANDS_LIMIT 6
const char *EXIT = "exit";
const char *PROMPT = ">";
const char *internalCommands[] = { "exit","pwd", "getpath", "setpath", "cd", "alias"};
char *homeDirectory;

int tokenCount = 0;
int aliasCount = 0;
char *result = NULL;
char *originalPath;
char path[1024];
int exitFlag = 0;

typedef struct{
	char *alias;
	char *command[CHAR_LIMIT];
	int commandCount;
}alias_t;

alias_t aliasData[ALIAS_LIMIT];

/*
* Name: parseInput(char *tokens[]);
*
* Description: Takes an array of "tokens" and parses
*	       Them as an executable program. Using
*	       a combination of fork() and execvp().
* 
 */
void parseInputAsExternal(char *tokens[]){
		char *programToExecute = tokens[0];
		pid_t proc;
		proc = fork();
			
		if(proc < 0){
			printf("Error: Fork Failed.");
		}
		else if(proc == 0){
			if(execvp(programToExecute, tokens) == -1){
				printf("Error: Command not found.\n");
				exit(0);
			}		
		}
		else{
			wait(NULL);
		}
}
int isInternalCommand(char *token){
	int result = 1;
	int i;
	for(i = 0; i < INTERNAL_COMMANDS_LIMIT; i++){
		result = strcmp(token,internalCommands[i]);
		if(result == 0)
			return result;
	}
	return result;
}
int createAlias(char *tokens[]){
	int i;
	alias_t aliasTok;
	if(tokens[1] != NULL && tokens[2] != NULL){
		aliasTok.alias = tokens[1];
		aliasTok.commandCount = 0;
		for(i = 2; i < tokenCount; i++){
			aliasTok.command[aliasTok.commandCount++] = tokens[i];
		}
		aliasData[aliasCount++] = aliasTok;
		return 0;
	}
	else{
		return -1;
	}
}
int isAlias(char *token){
	int i;
	printf("\n Alias Count: %d", aliasCount);
	for(i = 0; i < aliasCount; i++){
		if(strcmp(token,aliasData[i].alias) == 0)
			return 0;
	}	
	return -1;
}
alias_t getAlias(char *alias){
	int i;
	alias_t aliasTok;
	aliasTok.alias = "ERROR";
	aliasTok.command[0] = "ERROR";
	aliasTok.commandCount = 0;
	for(i = 0; i < aliasCount; i++){
		if(strcmp(alias,aliasData[i].alias) == 0)
			aliasTok = aliasData[i];
			return aliasTok;
		}	
	return aliasTok;
}
void processCommand(char *tokens[]){
	/* Check if command entered is internal or external to the shell. */
	if(isInternalCommand(tokens[0]) == 0){
		if(strcmp(tokens[0],internalCommands[0]) == 0){
			/* Execute Exit Command */
			exitFlag = 1;
		}
		/* Check if Command Entered is pwd */
		else if(strcmp(tokens[0], internalCommands[1]) == 0){
					/* Execute PWD Command */
			if(getcwd(path, sizeof(path)) != NULL)
				printf("\nCurrent Working Directory: %s \n",getcwd(path, sizeof(path)));
			else
				printf("\nError: \"%s\" failed.\n", internalCommands[1]);
			}
			/* Check if Command Entered is getpath */
			else if(strcmp(tokens[0], internalCommands[2]) == 0){
				/* Execute GETPATH Command */
				printf("\n Current Path: %s\n", getenv("PATH"));
			}
			/* Check if Command Entered is setpath */
			else if(strcmp(tokens[0], internalCommands[3]) == 0){
					/* Execute GETPATH Command */
					if(tokens[1] != NULL){
						if(setenv("PATH", tokens[1], 1) == 0){
							printf("\nPath Set to: %s\n", tokens[1]);
						}
					}
					else{
						printf("\nError: No Path Specified. \n");
					}
				}
				/* Check if Command Entered is cd */
			     else if(strcmp(tokens[0], internalCommands[4]) == 0){
					if(tokens[2] != NULL){
						printf("\nchdir(): Too many arguments.\n");
					}					
					else if(chdir(tokens[1]) < 0){
						perror("chdir()");
					}
						
				}
				/* Check if Command Entered is Alias */
				else if(strcmp(tokens[0], internalCommands[5]) == 0){
					printf("\n Alias Creation Success: %d\n",createAlias(tokens));
				}
			}
			else if(isAlias(tokens[0]) == 0){
			/* Check if Command entered is an alias.			*/
			alias_t aliasTok = getAlias(tokens[0]);
			
			printf("\nAlias: %s Command: %s", aliasTok.alias,aliasTok.command[0]);
			int y = 0;
			for(y = 0; y < aliasTok.commandCount; y++){
			
			}
			processCommand(aliasTok.command);
			
			}
			else{
				parseInputAsExternal(tokens);
			}
}
int aliasing() {
char input[CHAR_LIMIT];




 originalPath = getenv("PATH");
 homeDirectory = getenv ("HOME");
 chdir(homeDirectory);
 printf("\nOriginal Path: %s", originalPath);
	
	do{
		char *tokens[CHAR_LIMIT];
		tokenCount = 0;
	        printf("%s%s",getcwd(path,sizeof(path)),PROMPT);

	
		if(fgets(input,sizeof(input),stdin)){
			/* Remove any new-line or tab characters */

			result = strtok (input," \n\t");
		    /* Add user input to tokenized array  */
			
			if(result != NULL){
				tokens[tokenCount] = result;
				tokenCount++;
				
				/* Cycle through rest of user input toksdfsd/ enizing it. */
			
					while(result != NULL){
						printf("%s\n",result);
						result = strtok (NULL," \n\t");
						tokens[tokenCount] = result;
						tokenCount++;
				}
				
				processCommand(tokens);
			}
	
		}
		if(strncmp(input,EXIT,4) == 0 || feof(stdin)){
			setenv("PATH", originalPath, 1);
			printf("\nPath Restored To: %s\n",getenv("PATH"));
			exitFlag = 1;
		}
	}
	while(exitFlag==0);  
  return(0);
  
}
