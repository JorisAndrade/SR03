CC=gcc
CFLAGS=-O2 -Wall

all: client.out server.out clean 

client.out: client.o
	$(CC) -o $@ client.o

server.out: server.o
	$(CC) -o $@ server.o 

%.o : %.c 
	@echo "Compilation"
	$(CC) -o $@ -c $< -I.

clean :
	@echo "Cleaning"
	@rm -f princ *~ *.o
