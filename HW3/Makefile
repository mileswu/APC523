CFLAGS = -g -O2 -Wno-write-strings
CXXFLAGS = -g -O2
CC = g++

all: question1 question2 question3

question1: numericalrecipes/rk4.o numericalrecipes/nrutil.o numericalrecipes/bsstep.o question1.o
	$(CXX) -o $@ $^

question2: question2.o
	$(CXX) -o $@ $^

question3: question3.o
	$(CXX) -o $@ $^

clean:
	$(RM) *.o
	$(RM) numericalrecipes/*.o
	$(RM) *~
	$(RM) *#
