#include "matrix.h"

Matrix finitediff_matrix(int n);
Matrix f_vector(int n);
Matrix u_vector(int n);
void jacobi_iteration(Matrix& a, Matrix& last, Matrix& rhs);
