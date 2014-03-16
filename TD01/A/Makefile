CC=gcc
CFLAGS=-O2 -Wall

all: forkpipe.out clean 

forkpipe.out: forkpipe.o
	$(CC) -o $@ forkpipe.o

%.o : %.c 
	@echo "Compilation"
	$(CC) -o $@ -c $< -I.

clean :
	@echo "Cleaning"
	@rm -f princ *~ *.o
