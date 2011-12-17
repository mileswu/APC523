#include <iostream>
#include <fstream>
#include <sstream>
#include "jacobi.h"
#include <cmath>

using namespace std;
#define N 255
#define ITER 10

int logbase2(int n) {
	int lev = 0;
	while(n != 1) {
		n /= 2;
		lev++;
	}
	return(lev);
}

void multigrid(Matrix& a, Matrix& last, Matrix& rhs) {
	int n = a.cols;
	int level = logbase2(n-1);
	int n_coarse = pow((double)2.0, level-1) +1;

	for(int i=0; i<ITER; i++) {
		jacobi_iteration(a, last, rhs);
	}

	if(level > 2) {
		// Residual
		Matrix t(n, 1);
		for(int i=1; i<n-1; i++) {
			double temp;
			temp = -rhs(i,0);
			for(int j=0; j<n; j++) // Matrix-vector multiply
				temp += a(i, j) * last(j, 0);
			t(i, 0) = temp;
		}

		// Coarsen residual
		Matrix t_coarse(n_coarse,1);
		for(int i=0; i<n_coarse; i++) 
			t_coarse(i, 0) = t(2*i, 0); // Only keep even values

		// Recurse down to next grid
		Matrix e(n_coarse,1);
		Matrix matrix_coarse = finitediff_matrix(n_coarse);
		multigrid(matrix_coarse, e, t_coarse);

		// Interpolate E
		Matrix e_interpolate(n, 1);
		for(int i=1; i<n-1; i++) {
			if(i%2 == 0) // Copy even
				e_interpolate(i, 0) = e(i/2, 0);
			else // Interpolate odd
				e_interpolate(i, 0) = e(i/2, 0)*0.5 + e(i/2 + 1, 0)*0.5;
		}

		// Subtraction
		for(int i=1; i<n-1; i++) {
			last(i,0) -= e_interpolate(i, 0); 
		}
	}
	
	// Iterate again
	for(int i=0; i<ITER; i++) {
		jacobi_iteration(a, last, rhs);
	}
}

int main(int argv, char **argc) {
	// Creation of all the matrix and vectors
	Matrix matrix = finitediff_matrix(N+2);
	Matrix f = f_vector(N+2);
	Matrix u = u_vector(N+2);

	// Run
	multigrid(matrix, u, f);

	// Open output file for the data points
	ofstream fout;
	fout.open("multigrid.csv");
	
	for(int i=0; i<N+2; i++) {
		double x = 1.0/(N+1) *i;
		fout << x << " " << u(i, 0) << endl;
	}

	fout.close();

	return(0);
}
