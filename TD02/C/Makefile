CC=gcc
CFLAGS=-O2 -Wall -Werror

all: client.out server.out clean 

client.out: client.o
	$(CC) -o $@ client.o $(CFLAGS)

server.out: server.o
	$(CC) -o $@ server.o $(CFLAGS)

%.o : %.c 
	@echo "Compilation"
	$(CC) -o $@ -c $< -I.

clean :
	@echo "Cleaning"
	@rm -f princ *~ *.o
