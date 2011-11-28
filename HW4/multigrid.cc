#include "matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
#include <cmath>
#define N 255
#define PI 3.141592654
#define ITER 10

Matrix finitediff_matrix() {
	Matrix m(N+2, N+2);
	double invhsq = 1.0 / pow(1.0/(N+1),2);

	// Values for finite difference matrix
	for(int i=1; i<N+1; i++) {
		m(i, i) = invhsq * -2;
		m(i, i+1) = invhsq;
		m(i, i-1) = invhsq;
	}
	return(m);
}

Matrix f_vector() {
	Matrix m(N+2,1);

	//double invhsq = 1.0 / pow(1.0/(N+1),2);
	for(int i=0; i<N+2; i++) {
		double x = 1.0/(N+1) *i;
		// Values of f(x)
		m(i, 0) = -20 + 60*PI*x * cos(20*PI*x*x*x) - 1800*PI*PI*x*x*x*x * sin(20*PI*x*x*x);

	}
	// Boundary conditions
	//m(0, 0) -= invhsq;
	//m(N+1, 0) -= 3*invhsq;

	return(m);
}

Matrix u_vector() {
	Matrix m(N+2,1);

	for(int i=0; i<N+2; i++) {
		double x = 1.0/(N+1) *i;
		// Initial guess for u
		m(i, 0) = 1 + 2*x;
	}
	return(m);
}


void jacobi_iteration(Matrix& a, Matrix& last, Matrix& rhs) {
	// Performs one iteration of Jacobi's algorithm. (a = Matrix, last = last value of x, rhs = vector b)
	int n = a.cols;
	Matrix retval(n,1);

	for(int i=1; i<n-1; i++) {
		double sum = 0;
		for(int j=0; j<n; j++) {
			if(i == j) continue;
			sum += a(i,j) * last(j,0);
		}

		retval(i, 0) = (rhs(i,0) - sum) / a(i,i);
	}
	
	// Boundary conditions
	
	retval(0, 0) = last(0, 0);
	retval(n-1, 0) = last(n-1, 0);

	// Using &last to update the current iteration of x
	last = retval;
}

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
	int n_coarse = pow(2, level-1) +1;
	cout << "Level: " << level << endl;

	for(int i=0; i<ITER; i++) {
		jacobi_iteration(a, last, rhs);
	}

	if(level > 2) {
		// Residual
		Matrix t(n, 1);
		for(int i=1; i<n-1; i++) {
			double temp;
			temp = -rhs(i,0);
			for(int j=0; j<n; j++) {
				temp += a(i, j) * last(j, 0);
			}
			t(i, 0) = temp;
		}

		// Coarsen residual
		Matrix t_coarse(n_coarse,1);
		for(int i=0; i<n_coarse; i++) 
			//t_coarse(i, 0) = t(2*i, 0)/4.0 + t(2*i + 1, 0)/2.0 + t(2*i + 2, 0)/4.0;
			t_coarse(i, 0) = t(2*i, 0);

		// New E
		Matrix e(n_coarse,1);
		
		Matrix matrix_coarse(n_coarse, n_coarse);
		double invhsq = 1.0 / pow(1.0/(n_coarse-1),2);

		// Values for finite difference matrix
		for(int i=1; i<n_coarse-1; i++) {
			matrix_coarse(i, i) = invhsq * -2;
			matrix_coarse(i, i+1) = invhsq;
			matrix_coarse(i, i-1) = invhsq;
		}

		// Recurse down to next grid
		multigrid(matrix_coarse, e, t_coarse);

		// Interpolate E
		Matrix e_interpolate(n, 1);
		for(int i=1; i<n-1; i++) {
			if(i%2 == 0)
				e_interpolate(i, 0) = e(i/2, 0);
			else
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
	Matrix matrix = finitediff_matrix();
	Matrix f = f_vector();
	Matrix u = u_vector();

	// Open output file for the data points (to be used with plotting)
	ofstream fout;
	fout.open("multigrid.csv");

	multigrid(matrix, u, f);

	for(int i=0; i<N+2; i++) {
		double x = 1.0/(N+1) *i;
		fout << x << " " << u(i, 0) << endl;
	}

	fout.close();

	return(0);
}
