#include "vector.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <sstream>

Vector::Vector(int n) {
	size = n;
	//data = (double *) malloc(sizeof(double) * n);
	data = new double[n];

	for(int i=0; i<n; i++)
		data[i] = 0;
}

Vector::~Vector() {
	delete[] data;
}

Vector::Vector(const Vector& rhs) {
	size = rhs.size;
	data = new double[size];
	std::copy(rhs.data, rhs.data+size, data);
}
Vector& Vector::operator=(Vector& rhs) {
	Vector tmp(rhs);
	swap(data, tmp.data);
	swap(size, tmp.size);
	return *this;
}


string Vector::to_s() {
	stringstream s;
	s << "(";

	for(int i=0; i<size-1; i++)
		s << data[i] << ",";
	s << data[size-1];
	
	s << ")";
	return(s.str());
}

void Vector::print() {
	cout << to_s() << endl;
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
Vector operator- (Vector& a, Vector& b) {
	Vector temp = b*(-1.0);
	return(a + temp);
}

Vector operator* (Vector& a, double b) {
	Vector output(a.dimensions());
	for(int i=0; i<a.dimensions(); i++)
		output[i] = a[i]*b;
	
	return(output);
}


