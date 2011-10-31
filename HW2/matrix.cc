#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


Matrix::Matrix (int r, int c) {
	rows = r;
	cols = c;

	data = new double[r*c];
	for(int i=0; i<r*c; i++)
		data[i] = 0;
}

Matrix::~Matrix () {
	delete[] data;
}

Matrix::Matrix(const Matrix& rhs) {
	rows = rhs.rows;
	cols = rhs.cols;
	data = new double[rows*cols];
	std::copy(rhs.data, rhs.data + rows*cols, data);
}

Matrix& Matrix::operator=(Matrix& rhs) {
	Matrix tmp(rhs);
	swap(data, tmp.data);
	swap(cols, tmp.cols);
	swap(rows, tmp.rows);
	return *this;
}

double& Matrix::operator() (int r, int c) {
	return data[r*cols + c];
}
