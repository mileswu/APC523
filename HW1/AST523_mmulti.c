#include <stdio.h>
#include <stdlib.h>

#include "AST523_mmulti.h"

int main(int argc, char **argv) {
  //check for the correct number of input arguments
  if(argc != 4){
    fprintf(stderr, "There should be 3 arguments, nrowA (integer), ncolA (integer), and ncolB (integer) \n");
    fprintf(stderr, "Usage: %s nrowA ncolA ncolB \n", argv[0]);
    return 1;}
  int nrowA = atoi(argv[1]); int ncolA = atoi(argv[2]); 
  int nrowB = ncolA; int ncolB = atoi(argv[3]);
  int nrowC = nrowA; int ncolC = ncolB;
  //initialize matrices
  AST523_MATRIX *A = AST523_matrixNew(nrowA, ncolA);
  AST523_MATRIX *B = AST523_matrixNew(nrowB, ncolB);
  AST523_MATRIX *C = AST523_matrixNew(nrowC, ncolC);
  sin_init(A);
  sin_init(B);
  //perform multiplication
  matrix_multiply(A,B,C);
  //print output
  print_max(C);
  //clear memory
  AST523_matrixDel(A);
  AST523_matrixDel(B);
  AST523_matrixDel(C);
  return 0;
}


