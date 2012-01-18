CFLAGS = -g -Wall -O2

all: nbody

nbody: particles.o tree.o nbody.o
	$(CC) -o $@ $^ -lpng

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
