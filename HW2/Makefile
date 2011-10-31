CXXFLAGS = -g -Wall -O2
CC = g++

all: problem1 problem2

problem1: problem1.o matrix.o
	$(CC) -o $@ $^

problem2: problem2.o minimization.o vector.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
