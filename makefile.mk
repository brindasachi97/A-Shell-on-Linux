#This is a comment line
CC=gcc 
#CFLAGS will be the options passed to the compiler.
CFLAGS= -c -Wall 

all: shell

shell: shell2.o 
	$(CC) shell2.o -o shell -lreadline 
	
shell2.o: shell2.c
	$(CC) $(CFLAGS) shell2.c

clean:
	rm -rf *.o








