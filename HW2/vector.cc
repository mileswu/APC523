#include "vector.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <sstream>

Vector::Vector(int n) {
	size = n;
	data = new double[n];

	// Initialize to 0
	for(int i=0; i<n; i++)
		data[i] = 0;
}

Vector::~Vector() {
	delete[] data;
}

Vector::Vector(const Vector& rhs) {
	// Construction from another Vector
	size = rhs.size;
	data = new double[size];
	std::copy(rhs.data, rhs.data+size, data);
}

Vector& Vector::operator=(Vector& rhs) {
	// Assignment operator
	Vector tmp(rhs);
	swap(data, tmp.data);
	swap(size, tmp.size);
	return *this;
}


string Vector::to_s() {
	// Provides a string representation of the vector
	stringstream s;
	s << "(";

	for(int i=0; i<size-1; i++)
		s << data[i] << ",";
	s << data[size-1];
	
	s << ")";
	return(s.str());
}

void Vector::print() {
	// Prints the string to stdout
	cout << to_s() << endl;
}

int Vector::dimensions() {
	// Dimension of the vector
	return(size);
}

double& Vector::operator[] (int i) { 
	// Accessor operator
	return(data[i]);
}

Vector operator+ (Vector& a, Vector& b) {
	// Addition operator
	
	// Check both vectors are the same size
	assert(a.dimensions() == b.dimensions());
	Vector output(a.dimensions());

	for(int i=0; i<a.dimensions(); i++)
		output[i] = a[i] + b[i];	
	
	return(output);
}
Vector operator- (Vector& a, Vector& b) {
	// Subtraction operator

	Vector temp = b*(-1.0);
	return(a + temp);
}

Vector operator* (Vector& a, double b) {
	// Multiplication by scalar operator
	Vector output(a.dimensions());
	for(int i=0; i<a.dimensions(); i++)
		output[i] = a[i]*b;
	
	return(output);
}


