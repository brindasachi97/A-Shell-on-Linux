#include<stdio.h>
#include<stdlib.h>
#include<curses.h>

//#include<conio.h>
//#include<process.h>

int i,j,fg,ec2;
int choice;
char fn[20],e,c;
FILE *fp1,*fp2,*fp;
void Create();
void Append();
void Delete();
void Display();
void editor()
{
 do {
  //clrscr();
  printf("\n\t\t***** TEXT EDITOR *****");
  printf("\n\n\tMENU:\n\t-----\n ");
  printf("\n\t1.CREATE\n\t2.DISPLAY\n\t3.APPEND\n\t4.DELETE\n\t5.Go to Shell\n\t6.EXIT\n");
  printf("\n\tEnter your choice: ");
  scanf(" %c",&choice);
	printf("%c",choice);

  switch(choice)

  {
   case '1':
     Create();
     break;
   case '2':
     Display();
     break;
   case '3':
     Append();
     break;
   case '4':
     Delete();
     break;
   case '5':
	//my_shell();
	break;
   case '6':
     exit(0);
  }
 }while(choice<5);
}
void Create()
{
 fp1=fopen("temp.txt","w");
 printf("\n\tEnter the text and press '@' to save\n\n\t");
 while(1)
 {
  c=getchar();
  fputc(c,fp1);
  if(c == '@')
  {
   fclose(fp1);
   printf("\n\tEnter then new filename: ");
   scanf("%s",fn);
   fp1=fopen("temp.txt","r");
   fp2=fopen(fn,"w");
   while(!feof(fp1))
   {
    c=getc(fp1);
    putc(c,fp2);
   }
   fclose(fp2);
   break;
  }}
}
void Display()
{
  printf("\n\tEnter the file name: ");
  scanf("%s",fn);
  fp1=fopen(fn,"r");
  if(fp1==NULL)
  {
   printf("\n\tFile not found!");
   goto end1;
  }
  while(!feof(fp1))
  {
   c=getc(fp1);
   printf("%c",c);
  }
end1:
  fclose(fp1);
  printf("\n\n\tPress any key to continue...");
  getchar();
}
void Delete()
{
  printf("\n\tEnter the file name: ");
  scanf("%s",fn);
  fp1=fopen(fn,"r");
  if(fp1==NULL)
  {
   printf("\n\tFile not found!");
   goto end2;
  }
  fclose(fp1);
  if(remove(fn)==0)
  {
   printf("\n\n\tFile has been deleted successfully!");
   goto end2;
  }
  else
   printf("\n\tError!\n");
end2: printf("\n\n\tPress any key to continue...");
  getchar();
}

void Append()
{
  printf("\n\tEnter the file name: ");
  scanf("%s",fn);
  fp1=fopen(fn,"r");
  if(fp1==NULL)
  {
   printf("\n\tFile not found!");
   goto end3;
  }
  while(!feof(fp1))
  {
   c=getc(fp1);
   printf("%c",c);
  }
  fclose(fp1);
  printf("\n\tType the text and press 'tab button and then press enter' to append.\n");
  fp1=fopen(fn,"a");
  while(1)
  {
   c=getchar();
   if(c=='\t')
    goto end3;
   if(c=='\n')
   {
    printf("\n\t");
    fputc(c,fp1);
   }
   else
   {
    printf("%c",c);
    fputc(c,fp1);
   }
  }
end3: fclose(fp1);
  getchar();
}
