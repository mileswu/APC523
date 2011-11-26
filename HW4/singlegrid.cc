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

Matrix *finitediff_matrix() {
	Matrix *m_ptr = new Matrix(N, N);
	double invhsq = 1.0 / pow(1.0/(N+1),2);

	// Values for finite difference matrix
	for(int i=0; i<N; i++) {
		(*m_ptr)(i, i) = invhsq * -2;
		if(i+1 < N)
			(*m_ptr)(i, i+1) = invhsq;
		if(i-1 >= 0)
			(*m_ptr)(i, i-1) = invhsq;
	}
	return(m_ptr);
}

Matrix *f_vector() {
	Matrix *m_ptr = new Matrix(N,1);

	double invhsq = 1.0 / pow(1.0/(N+1),2);
	for(int i=0; i<N; i++) {
		double x = 1.0/(N+1) * (i+1);
		// Values of f(x)
		(*m_ptr)(i, 0) = -20 + 60*PI*x * cos(20*PI*x*x*x) - 1800*PI*PI*x*x*x*x * sin(20*PI*x*x*x);

		// Boundary conditions
		if(i==0) 
			(*m_ptr)(i, 0) -= invhsq;
		if(i==N-1)
			(*m_ptr)(i, 0) -= 3*invhsq;
	}
	return(m_ptr);
}

Matrix *u_vector() {
	Matrix *m_ptr = new Matrix(N,1);

	for(int i=0; i<N; i++) {
		double x = 1.0/(N+1) * (i+1);
		// Initial guess for u
		(*m_ptr)(i, 0) = 1 + 2*x;
	}
	return(m_ptr);
}


void jacobi_iteration(Matrix& a, Matrix& last, Matrix& rhs) {
	// Performs one iteration of Jacobi's algorithm. (a = Matrix, last = last value of x, rhs = vector b)
	Matrix retval(N,1);

	for(int i=0; i<N; i++) {
		double sum = 0;
		for(int j=0; j<N; j++) {
			if(i == j) continue;
			sum += a(i,j) * last(j,0);
		}

		retval(i, 0) = (rhs(i,0) - sum) / a(i,i);
	}

	// Using &last to update the current iteration of x
	last = retval;
}

int main(int argv, char **argc) {
	// Argument parsing for no of iterations
	int iter_num;
	if(argv != 2) {
		cout << "Usage: " << argc[0] << " <no of iterations>" << endl;
		return(1);
	}
	iter_num = atoi(argc[1]);
	if(iter_num < 1) {
		cout << "Invalid argument for no of iterations" << endl;
		cout << "Usage: " << argc[0] << " <no of iterations>" << endl;
		return(1);
	}

	// Creation of all the matrix and vectors
	Matrix *matrix = finitediff_matrix();
	Matrix *f = f_vector();
	Matrix *u = u_vector();

	// Open output file for the data points (to be used with plotting)
	ofstream fout;
	stringstream ss;
	ss << "singlegrid_n_" << iter_num << ".csv";
	cout << "Writing to: " << ss.str().c_str() << endl;
	fout.open(ss.str().c_str());
	
	for(int i=0; i<iter_num; i++) {
		jacobi_iteration(*matrix, *u, *f);
		if(i%100 == 0)
			cout << "Iteration: " << i << "/" << iter_num << endl;
	}

	for(int i=0; i<N; i++) {
		double x = 1.0/(N+1) * (i+1);
		fout << x << " " << (*u)(i, 0) << endl;
	}

	fout.close();

	// Free memory
	delete matrix;
	delete u;
	delete f;

	return(0);
}
