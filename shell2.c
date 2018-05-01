// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<curses.h>
#include "editor2.c"
#include "alias.c"
#include "prjTest.c"
#include "ownps.c"
#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
#define HISTORY_COUNT 20
#define BUFFER 300
// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int zeroSize1();
char *path1[100];
int i,j,ec,fg,ec2;
char fn[20],e,c;
char ch;
FILE *fp1,*fp2,*fp;
void Create();
void Append();
void Delete();
void Display();
void editor();
int min,max;
 
static char *token;
static char copy[BUFFER];
// Greeting shell during startup
void init_shell(){
    clear();
    printf("\n\n\n\n******************************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\n\n******************************************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    setenv("SHELL","myShell",1);
    printf("\n");
    sleep(1);
    clear();
}
 
// Function to take input
int takeInput(char* str){
    char* buf;
 
    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}
 
// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}
 
// Function where the system command is executed
void execArgs(char** parsed){
    pid_t pid = fork(); //Forking a child
 
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
	/*if(*parsed=="history")
		printf("History:\n");*/
        if (execvp(parsed[0], parsed) < 0) {
           // printf("Could not execute command..\n");
        }
        exit(0);
    } else {
        wait(NULL);  //Waiting for child to terminate
        return;
    }
}
 
// Function where the Piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe){
    int pipefd[2];  //0-read end;1-write end
    pid_t p1, p2;
 
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }
 
    if (p1 == 0) {  //Child1 (Write to fd[1])
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
 
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        p2 = fork(); //Parent executing
 
        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        } 
        if (p2 == 0) { //Child2 (Read at fd[0]
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        }else {  //Parent wait for 2 children
            wait(NULL);
            wait(NULL);
        }
    }
}
 
// Help command builtin
void openHelp(){
    puts("\n***WELCOME TO MY SHELL HELP***"
        "\nList of Commands supported:"
        "\ncd"
        "\nls"
        "\nexit"
	"\ncp"
	"\nmv\ncat\nmore\nless\nmkdir\nrm\nps\ngcc\ndate\nhistory\naliasing\n"
      	"\n");
 
    return;
}
 
// Function to execute builtin commands
int owninputStringHandler(char** parsed){
    int NoOfOwninputStrings = 8, i, switchOwnArg = 0;
    char* ListOfOwninputStrings[NoOfOwninputStrings];
    char* username;
 
    ListOfOwninputStrings[0] = "exit";
    ListOfOwninputStrings[1] = "cd";
    ListOfOwninputStrings[2] = "help";
    ListOfOwninputStrings[3] = "hello";
    ListOfOwninputStrings[4] = "Editor";
    ListOfOwninputStrings[5] = "aliasing";
    ListOfOwninputStrings[6] = "lsz";
    ListOfOwninputStrings[7] = "myps";
 
    for (i = 0; i < NoOfOwninputStrings; i++) {
        if (strcmp(parsed[0], ListOfOwninputStrings[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }
    
    switch (switchOwnArg) {
    case 1:
        printf("\nGoodbye\n");
        exit(0);
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
        openHelp();
        return 1;
    case 4:
        username = getenv("USER");
        printf("\nHello %s.\nWelcome to my Shell\n",username);
        return 1;
    case 5:
        editor();
        return 1;
    case 6:
	aliasing();
	return 1;
    case 7:
	zeroSize1();
	return 1;
    case 8:
	ownps();
	return 1;
    default:
        break;
    }
 
    return 0;
}

int zeroSize1(){
	printf("Enter path\n");
	scanf("%s",&path1);
	zeroSize(path1);
}
// function for finding pipe
int parsePipe(char* str, char** strpiped){
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }
 
    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}
 
// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;
 
    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
 
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}
 
int processString(char* str, char** parsed, char** parsedpipe){
 
    char* strpiped[2];
    int piped = 0;
 
    piped = parsePipe(str, strpiped);
 
    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
 
    } else {
 
        parseSpace(str, parsed);
    }
 
    if (owninputStringHandler(parsed))
        return 0;
    else
        return 1 + piped;
}

int history(char *hist[], int current){
        int i = current;
        int hist_num = 1;

        do {
                if (hist[i]) {
                        printf("%4d  %s\n", hist_num, hist[i]);
                        hist_num++;
                }

                i = (i + 1) % HISTORY_COUNT;

        } while (i != current);

        return 0;
}
/***********************************************************************************/
int main(){
	char inputString[MAXCOM], *parsedArgs[MAXLIST];
	char* parsedArgsPiped[MAXLIST];
	
	char *redirect_pos;

	char *hist[HISTORY_COUNT];
	int execFlag = 0;
	int op;
	int i, current = 0;
	init_shell();
	for (i = 0; i < HISTORY_COUNT; i++)
		hist[i] = NULL;
 
	printf("\nChoose an option and Press Enter to continue \n 1. Execute unix commands with History feature \n 2. Editor\n");
	scanf("%d", &op);
	switch(op){
		
case 1: {
	while(1){
		printDir();
		if (!takeInput(inputString))
			//continue;
       		execFlag = processString(inputString, parsedArgs, parsedArgsPiped);
		strcpy(copy,inputString);
		token=strtok(inputString, " \t\n()<>|&;");
		if((redirect_pos=strchr(copy,'>'))){
			if(*(redirect_pos+1)=='>')
				redirect_output('a');
			else
				redirect_output('w');
		}
			
		
       
	        if (execFlag == 1) //Simple Command
	            execArgs(parsedArgs);
 
	        if (execFlag == 2) //Pipe
	            execArgsPiped(parsedArgs, parsedArgsPiped);
        
	        free(hist[current]);
	        hist[current] = strdup(inputString);
	        current = (current + 1) % HISTORY_COUNT;
	        if (strcmp(inputString, "history") == 0)
			history(hist, current);
	        else if (strcmp(inputString, "quit") == 0)
		        break;
	}
	break;
}

case 2: 
	editor();
	break;
    
}


return 0;
}

void redirect_output(char flag) {
  pid_t pid;
  char *args[BUFFER];
  int arg_num;
  FILE* fp;
  int status;

  status = 0;
  arg_num = 0;
  pid = fork(); 

  //An error occoured with the fork
  if(pid == -1) {
    //perror("");
    ;//exit(EXIT_FAILURE);
  }
  //Child process
  else if(pid == 0) {
    args[arg_num] = token;

    while(token != NULL) {
      token = strtok(NULL, " \t\n()<>|&;");
      arg_num++;
      args[arg_num] = token;
    }

    //Second to last token should be file name, redirect output
    fp = freopen(args[arg_num - 1], &flag, stdout);
    if (fp == NULL) {
      //perror("");
      return;
    }
    args[arg_num - 1] = NULL;

    //Run the program specified
    if((execvp(args[0], args) == -1)) {
	;
         //perror("");
    }   

    fclose(fp); 
    _exit(EXIT_SUCCESS); 
  } 
  //Parent process
  else {
    if (wait(&status) == -1) {
      //perror("");
      return;
    }

    //Checks 'status' for normal exit
    if (WIFEXITED(status) == 0) {
      printf("Child process did not exit normally.");
    } 
  }
}







