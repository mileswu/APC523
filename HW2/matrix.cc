#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


Matrix::Matrix (int r, int c) {
	rows = r;
	cols = c;

	data = (double *)malloc(sizeof(double) * r * c);
	for(int i=0; i<r*c; i++)
		data[i] = 0;
}

Matrix::~Matrix () {
//	cout << "Hi destructing matrix (" << rows << "x" << cols << ")" << endl;
	free(data);
}


double& Matrix::operator() (int r, int c) {
	return data[r*cols + c];
}
