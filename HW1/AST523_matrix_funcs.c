#include <stdio.h>
#include "AST523_mmulti.h"
#include "math.h"

// Block sized use in the blocking matrix multiplication. 32 seemed a good number
#define BLOCKSIZE 32

//initialize a matrix using sin
void sin_init(AST523_MATRIX *M){
  int row, col;
  for(row = 0; row < M->nrow; row++) {
    for(col = 0; col < M->ncol; col++) {
      M->val[row][col] = sin(col + (M->ncol)*row); 
    }
  }
}

//initialize a matrix of zeros
void zero_init(AST523_MATRIX *M){
  int row, col;
  for(row = 0; row < M->nrow; row++) {
    for(col = 0; col < M->ncol; col++) {
      M->val[row][col] = 0; 
    }
  }
}

//performs the matrix multiplication A*B = C. Matrices A&B are unchanged
//while the answer is stored in C.
void matrix_multiply(AST523_MATRIX *A, AST523_MATRIX *B, AST523_MATRIX *C){
  int rowA, colA, colB;
  int rowA_block, colA_block, colB_block;
  zero_init(C);

  // Block-level loops
  for(rowA_block = 0; rowA_block < A->nrow; rowA_block += BLOCKSIZE) { 
    for(colB_block = 0; colB_block < B->ncol; colB_block += BLOCKSIZE){ 
      for(colA_block = 0; colA_block < A->ncol; colA_block += BLOCKSIZE){

  // Sub-matrix loops
        for(rowA = rowA_block; rowA < A->nrow && rowA < rowA_block + BLOCKSIZE; rowA++)  
          for(colB = colB_block; colB < B->ncol && colB < colB_block + BLOCKSIZE; colB++)  
            for(colA = colA_block; colA < A->ncol && colA < colA_block + BLOCKSIZE; colA++) 
C->val[rowA][colB] += (A->val[rowA][colA])*(B->val[colA][colB]);
      }
    }
  }
}

//prints the maximum value of a matrix, along with the row and column number.
void print_max(AST523_MATRIX *M){
  int row, col, max_row = 0, max_col = 0;
  double max_val = M->val[0][0];
  for(row = 0; row < M->nrow; row++){
    for(col = 0; col < M->ncol; col++){
      if(M->val[row][col] > max_val) {
	max_val = M->val[row][col];
	max_row = row;
	max_col = col;
      }
    }
  }
  printf("max: %lf, row: %d, col: %d \n", max_val, max_row, max_col);
}
