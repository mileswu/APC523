CFLAGS = -g -Wall -O0 #-fopenmp -pg
LDFLAGS = #-fopenmp -pg
CC = gcc

LIBS = -lm
PROG_MATRIX = AST523_mmulti
PROG_SERIES = AST523_hseries
OBJS_MATRIX = AST523_mmulti.o AST523_matrix.o AST523_matrix_funcs.o
OBJS_SERIES = AST523_hseries.o

all: $(PROG_MATRIX) $(PROG_SERIES)

$(PROG_MATRIX) : $(OBJS_MATRIX)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(PROG_SERIES) : $(OBJS_SERIES)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJS_MATRIX) : AST523_mmulti.h

clean:
	$(RM) *.o
	$(RM) *~
	$(RM) *#

# The dependencies of the .c files on the .o files are inferred by Make.
# @ copies the program name (target) ^ copies the dependent file(s)
# type make clean to remove object files, etc.
# Use -wall flag to print warnings.
# Use -g flag to enable memory leak detection (e.g. using Valgrind).
# Use -pg to enable profiling information (e.g. using gprof).
# Use -fopenmp when compiling with OpenMP directives.