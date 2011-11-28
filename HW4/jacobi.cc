#include "matrix.h"
#include <iostream>

using namespace std;
#include <cmath>

#define N 255
#define PI 3.141592654

Matrix finitediff_matrix(int n) {
	Matrix m(n, n);
	double invhsq = 1.0 / pow(1.0/(n-1),2);

	// Values for finite difference matrix
	for(int i=1; i<n-1; i++) {
		m(i, i) = invhsq * -2;
		m(i, i+1) = invhsq;
		m(i, i-1) = invhsq;
	}
	return(m);
}

Matrix f_vector(int n) {
	Matrix m(n,1);

	for(int i=0; i<n; i++) {
		double x = 1.0/(n-1) *i;
		// Values of f(x)
		m(i, 0) = -20 + 60*PI*x * cos(20*PI*x*x*x) - 1800*PI*PI*x*x*x*x * sin(20*PI*x*x*x);
	}

	return(m);
}

Matrix u_vector(int n) {
	Matrix m(n,1);

	for(int i=0; i<n; i++) {
		double x = 1.0/(n-1) *i;
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

