CXXFLAGS = -g -Wall -O2

all: singlegrid

singlegrid: singlegrid.o matrix.o
	$(CXX) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
