CFLAGS = -g -Wall

all: nbody

nbody: particles.o tree.o nbody.o
	$(CC) $(CFLAGS) -o $@ $^ -lpng

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
