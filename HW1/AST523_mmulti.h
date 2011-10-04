typedef struct {
  double* val_s; // space for 2-D array of values
  double** val;  // space for pointers to rows
  int nrow, ncol; // number of rows/columns
} AST523_MATRIX;

AST523_MATRIX *AST523_matrixNew(int nrow, int ncol);
void AST523_matrixDel(AST523_MATRIX *v);
void zero_init(AST523_MATRIX *M);
void sin_init(AST523_MATRIX *M);
void matrix_multiply(AST523_MATRIX *A, AST523_MATRIX *B, AST523_MATRIX *C);
void print_max(AST523_MATRIX *M);

 /*
  * Globals
  */
