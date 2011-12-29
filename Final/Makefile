CFLAGS = -g -Wall -O2

all: particles

particles: particles.o
	$(CC) -o $@ $^ -lpng

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#
