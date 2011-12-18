CXXFLAGS = -g -Wall -O2

all: question1 question2

question1: question1.o
	$(CXX) -o $@ $^

question2: question2.o
	$(CXX) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
