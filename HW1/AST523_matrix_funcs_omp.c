#include <stdio.h>
#include "AST523_mmulti.h"
#include "math.h"

//initialize a matrix using sin
void sin_init(AST523_MATRIX *M){
  int row, col;
  #pragma omp parallel for private(row, col)
  for(row = 0; row < M->nrow; row++) {
    for(col = 0; col < M->ncol; col++) {
      M->val[row][col] = sin(col + (M->ncol)*row); 
    }
  }
}

//initialize a matrix of zeros
void zero_init(AST523_MATRIX *M){
  int row, col;
  #pragma omp parallel for private(row, col)
  for(row = 0; row < M->nrow; row++) {
    for(col = 0; col < M->ncol; col++) {
      M->val[row][col] = 0; 
    }
  }
}

//performs the matrix multiplication A*B = C. Matrices A&B are unchanged
//while the answer is stored in C.
void matrix_multiply(AST523_MATRIX *A, AST523_MATRIX *B, AST523_MATRIX *C){
  zero_init(C);  
  #pragma omp parallel 
  {
    int rowA, colA, colB;
    
    AST523_MATRIX *TEMP;
    TEMP = AST523_matrixNew(C->nrow, C->ncol);
    zero_init(TEMP);
    
    #pragma omp for
    for(rowA = 0; rowA < A->nrow; rowA++) { 
      for(colB = 0; colB < B->ncol; colB++){ 
        for(colA = 0; colA < A->ncol; colA++){ 
  	TEMP->val[rowA][colB] += (A->val[rowA][colA])*(B->val[colA][colB]);
        }
      }
    }
    
    for(rowA = 0; rowA < A->nrow; rowA++) { 
      for(colB = 0; colB < B->ncol; colB++){ 
        #pragma omp critical(add)
        {
          C->val[rowA][colB] += TEMP->val[rowA][colB];
        }
        
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
