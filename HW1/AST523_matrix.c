#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "AST523_mmulti.h"

//create a new AST523_matrix
AST523_MATRIX *AST523_matrixNew(int nrow, // number of rows
				int ncol) // number of columns
{
  AST523_MATRIX *M = malloc(sizeof(AST523_MATRIX));
  assert(M != NULL);
  M->val = malloc(nrow*sizeof(double*)); //assign of pointers to rows
  M->val_s = malloc(ncol*nrow*sizeof(double)); //array of matrix values
  int i;
  for (i = 0; i < nrow; i++)// set pointers to rows of data
    M->val[i] = M->val_s + i*ncol;
  M->nrow = nrow;
  M->ncol = ncol;

  return M;
}

//delete AST523_matrix
void AST523_matrixDel(AST523_MATRIX *M)
{
  if (M == NULL) return;
  
  free(M->val);
  free(M->val_s);
  free(M);
}
