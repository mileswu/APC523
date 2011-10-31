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
	Matrix *m_ptr = new Matrix(N,1);
	for(int i=0; i<N; i++) {
		(*m_ptr)(i, 0) = sin((i+1)* j * PI / (N+1));
	}
	return(m_ptr);
}

Matrix *b_vector(int j) {
	double lambda = 2 * (1 - cos(j * PI / (N+1)));

	Matrix *m_ptr = x_vector(j);
	for(int i=0; i<N; i++) {
		(*m_ptr)(i, 0) *= lambda;
	}
	return(m_ptr);
}

void jacobi_iteration(Matrix& a, Matrix& last, Matrix& rhs) {
	Matrix retval(N,1);

	for(int i=0; i<N; i++) {
		double sum = 0;
		for(int j=0; j<N; j++) {
			if(i == j) continue;
			sum += a(i,j) * last(j,0);
		}

		retval(i, 0) = (rhs(i,0) - sum) / a(i,i);
	}

	last = retval;
}

double error_comparison(Matrix &a, Matrix &b) {
	double error = -1, cur_error;
	for(int i=0; i<N; i++)
		cur_error = fabs(a(i,0) - b(i,0));
		if(cur_error < error || error == -1)
			error = cur_error;
	return(error);
}

void print_vector(Matrix &v) {
	for(int i=0; i<N; i++)
		cout << v(i,0) << ",";
	cout << endl;
}

int main(int argv, char **argc) {
	int j;
	if(argv != 2) {
		cout << "Usage: " << argc[0] << " <value for j (the eigenvalue)>" << endl;
		return(1);
	}
	j = atoi(argc[1]);

	Matrix *t = t_matrix();
	Matrix *x = x_vector(j);
	Matrix *b = b_vector(j);
	Matrix *x_iter = new Matrix(N, 1);

	ofstream fout;
	stringstream ss;
	ss << "jacobi_j_" << j << ".csv";
	cout << "Writing to: " << ss.str().c_str() << endl;
	fout.open(ss.str().c_str());
	
	for(int i=0; i<10000; i++) {
		double error = error_comparison(*x_iter, *x);
		if(i%100 == 0)
			fout << i << " " << error << endl;

		jacobi_iteration(*t, *x_iter, *b);
	}

	delete t;
	delete x;
	delete b;
	delete x_iter;

	return(0);
}
