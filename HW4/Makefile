CXXFLAGS = -g -Wall -O2

all: singlegrid multigrid

singlegrid: singlegrid.o matrix.o
	$(CXX) -o $@ $^

multigrid: multigrid.o matrix.o
	$(CXX) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
