CC=gcc
CFLAGS=-O2 -Wall

all: clio.out serv.out clean 

clio.out: clio.o
	$(CC) -o $@ clio.o $(CFLAGS)

serv.out: serv.o
	$(CC) -o $@ serv.o $(CFLAGS)

%.o : %.c 
	@echo "Compilation"
	$(CC) -o $@ -c $< -I.

clean :
	@echo "Cleaning"
	@rm -f princ *~ *.o
