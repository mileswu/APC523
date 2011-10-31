#include "matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
#include <cmath>

#define N 100
#define PI 3.141592654

Matrix *t_matrix() {
	// Create the NxN T matrix
	Matrix *m_ptr = new Matrix(N, N);
	for(int i=0; i<N; i++) {
		(*m_ptr)(i, i) = 2;
		if(i-1 >= 0)
			(*m_ptr)(i, i-1) = -1;
		if(i+1 < N)
			(*m_ptr)(i, i+1) = -1;
	}
	return(m_ptr);
}

Matrix *x_vector(int j) {
	// Create the final answer x vector (j for which eigenvalue)
	Matrix *m_ptr = new Matrix(N,1);
	for(int i=0; i<N; i++) {
		(*m_ptr)(i, 0) = sin((i+1)* j * PI / (N+1));
	}
	return(m_ptr);
}

Matrix *b_vector(int j) {
	// Create the RHS b vector (j for which eigenvalue)
	double lambda = 2 * (1 - cos(j * PI / (N+1)));

	// It is the same as the x vector, except multiplied by the eigenvalue
	Matrix *m_ptr = x_vector(j);
	for(int i=0; i<N; i++) {
		(*m_ptr)(i, 0) *= lambda;
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

double error_comparison(Matrix &a, Matrix &b) {
	// Calculate the error between two vectors
	double error = -1, cur_error;
	for(int i=0; i<N; i++)
		cur_error = fabs(a(i,0) - b(i,0));
		if(cur_error < error || error == -1)
			error = cur_error;
	return(error);
}

void print_vector(Matrix &v) {
	// Print a vector for debug purposes
	for(int i=0; i<N; i++)
		cout << v(i,0) << ",";
	cout << endl;
}

int main(int argv, char **argc) {
	// Argument parsing for j (the eigenvalue)
	int j;
	if(argv != 2) {
		cout << "Usage: " << argc[0] << " <value for j (the eigenvalue), eg. 1>" << endl;
		return(1);
	}
	j = atoi(argc[1]);
	if(j < 1 || j > 100) {
		cout << "Invalid argument for j." << endl;
		cout << "Usage: " << argc[0] << " <value for j (the eigenvalue), eg. 1>" << endl;
		return(1);
	}

	// Creation of all the matrix and vectors
	Matrix *t = t_matrix();
	Matrix *x = x_vector(j);
	Matrix *b = b_vector(j);
	Matrix *x_iter = new Matrix(N, 1);

	// Open output file for the data points (to be used with plotting)
	ofstream fout;
	stringstream ss;
	ss << "jacobi_j_" << j << ".csv";
	cout << "Writing to: " << ss.str().c_str() << endl;
	fout.open(ss.str().c_str());
	
	double error;
	for(int i=0; i<10000; i++) {
		error = error_comparison(*x_iter, *x);

		// Output error every 100 iterations to the file
		if(i%100 == 0)
			fout << i << " " << error << endl;

		if(i%1000 == 0)
			cout << "Iteration " << i+1 << "/10000. Error = " << error << endl;

		jacobi_iteration(*t, *x_iter, *b);
	}
	cout << "Iteration " << 10000 << "/10000. Error = " << error << endl;

	// Free memory
	delete t;
	delete x;
	delete b;
	delete x_iter;

	return(0);
}
