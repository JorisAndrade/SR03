CC=gcc
CFLAGS=-O2 -Wall

all: clio.out sero.out clean 

clio.out: clio.o
	$(CC) -o $@ clio.o

sero.out: sero.o
	$(CC) -o $@ sero.o

%.o : %.c 
	@echo "Compilation"
	$(CC) -o $@ -c $< -I.

clean :
	@echo "Cleaning"
	@rm -f princ *~ *.o
