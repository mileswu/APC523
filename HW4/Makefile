CXXFLAGS = -g -Wall -O2

all: singlegrid multigrid

singlegrid: singlegrid.o matrix.o jacobi.o
	$(CXX) -o $@ $^

multigrid: multigrid.o matrix.o jacobi.o
	$(CXX) -o $@ $^

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
