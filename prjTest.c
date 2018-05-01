//Recursively traverse a directory
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<limits.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<error.h>

long int size=0;

char *path1[100];
int count=0;
struct stat st;

long int findSize(const char* fileName){
	
	if(stat(fileName,&st)==0)
		return(st.st_size);
	else
		return -1;
		
}
void ListDirectory(const char *DirName, int Level, int min, int max){
	//printf("1Size=%d,Min=%d,Max=%d,%s\n",size,min,max,DirName);	
	DIR *Dir;
	char cwd[300];
	struct dirent *DirEntry;
	//printf("%s\n",DirName);
	if(!(Dir=opendir(DirName))){
		//printf("Size=%d,Min=%d,Max=%d\n",size,min,max);
		perror("Directory cant be opened\n");
		//exit(0);	
	}
	//printf("Size=%d,Min=%d,Max=%d\n",size,min,max);

	while((DirEntry=readdir(Dir))!=NULL){
		chdir(DirName);
		//printf("*********%s\n",DirName);
		size=findSize(DirEntry->d_name);
               
	//	printf("Size=%d,Min=%d,Max=%d\n",size,min,max);
		if(DirEntry->d_type==DT_DIR){	
			chdir(DirName);		
			char DirPath[1024];
			if(strcmp(DirEntry->d_name,".")==0 || strcmp(DirEntry->d_name,"..")==0)
				continue;
			ListDirectory(DirPath,Level+2,min,max);
		}
		else if((DirEntry->d_type==DT_REG) && (size>=min) && (size<=max) ){
			//printf("SIZE=%d\n",size);
//printf("Size=%d,Min=%d,Max=%d\n",size,min,max);
			printf("\n\n\n%s\t\n\n\n%ld\n",DirEntry->d_name,size);
			//printf("Size=%d,Min=%d,Max=%d\n",size,min,max);
			count++;
			//printf("The curent file Count=%d\n",count);
		}
	}
	closedir(Dir);	
}
	

int zeroSize(char *path[]){
long int min, max;
	
	printf("Enter min size\n");
	scanf("%d",&min);
	printf("Enter max size\n");
	scanf("%d",&max);
	//zeroSize(path1,min,max);
	
	ListDirectory(path1,0, min, max);
/*
	printf("Value of DT_BLK which is a block device is %d\n",DT_BLK);
	printf("Value of DT_CHR which is a character device is %d\n",DT_CHR);
	printf("Value of DT_DIR which is a directory is %d\n",DT_DIR);
	printf("Value of DT_FIFO which is a named pipe is %d\n",DT_FIFO);
	printf("Value of DT_LNK which is a symbolic link is %d\n",DT_LNK);
	printf("Value of DT_REG which is a regular file is %d\n",DT_REG);
	printf("Value of DT_SOCK which is a UNIX Domain Socket is %d\n",DT_SOCK);
	printf("Value of DT_UNKNOWN whose file type is unknown is %d\n",DT_UNKNOWN);
	printf("\n\n\n\n\n\n\n");
*/
	
}
/*
int main(){
printf("Enter path\n");
	scanf("%s",&path1);
	zeroSize(path1);
}
*/
/* 
*	DT_BLK	This is a block device
*	DT_CHR	This is a character deice
*	DT_DIR	This is a directory
*	DT_FIFO	This is a named pipe (FIFO)
*	DT_LNK	This is a symbolic link
*	DT_REG	This is a regular file
*/
