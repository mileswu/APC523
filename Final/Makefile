CFLAGS = -g -Wall -O2 -fopenmp

all: nbody

nbody: particles.o tree.o nbody.o
	$(CC) $(CFLAGS) -o $@ $^ -lpng

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
