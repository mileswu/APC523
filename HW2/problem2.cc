#include "minimization.h"
#include <iostream>
#include <cmath>
#include <complex>

using namespace std;


double f(Vector v) {
	// Function in problem
	complex<double> e (0.5, 0);
	complex<double> one (1.0, 0);

	// Shove vector values into complex number
	complex<double> w (v[0], v[1]);

	complex<double> retval;
	complex<double> wminus1squared = (w - one) * (w - one);
	retval = e*e*(one - w*w/e)*wminus1squared*wminus1squared - (one - wminus1squared)*w*w*w*w;

	// Take abs value squared
	return(retval.imag()*retval.imag() + retval.real()*retval.real());
}

int main() {
	// Start vector
	Vector x0(2);
	x0[0] = 0.4;
	x0[1] = 0.4;

	// Run powell minimization
	Vector output = powell(x0, f);

	// Print answer
	cout << "Answer is: " << output[0] << " + " << output[1] << "i" << endl;
	return(0);
}



