CFLAGS = -g -Wall -O2
CC = g++

all: problem1

problem1: problem1.o matrix.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
