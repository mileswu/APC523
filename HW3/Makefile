CXXFLAGS = -g -Wall -O2 -I./gsl/
CC = g++

all: question1 question2 question3

gsl/.libs/libgsl.a: 

question1: question1.o gsl/.libs/libgsl.a
	$(CC) -o $@ $^

question2: question2.o
	$(CC) -o $@ $^

question3: question3.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
