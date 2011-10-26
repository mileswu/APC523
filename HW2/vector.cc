#include "vector.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

using namespace std;

Vector::Vector(int n) {
	size = n;
	data = (double *) malloc(sizeof(double) * n);

	for(int i=0; i<n; i++)
		data[i] = 0;
}

Vector::~Vector() {
	cout << "Freeing: ";
	print();
	free(data);
}

void Vector::print() {
	cout << "(";
	for(int i=0; i<size; i++)
		cout << data[i] << ",";
	cout << ")" << endl;
}

int Vector::dimensions() {
	return(size);
}

double& Vector::operator[] (int i) { 
	return(data[i]);
}

Vector operator+ (Vector& a, Vector& b) {
	assert(a.dimensions() == b.dimensions());
	Vector output(a.dimensions());

	for(int i=0; i<a.dimensions(); i++)
		output[i] = a[i] + b[i];	
	
	return(output);
}

Vector operator* (Vector& a, double b) {
	Vector output(a.dimensions());
	for(int i=0; i<a.dimensions(); i++)
		output[i] = a[i]*b;
	
	return(output);
}


