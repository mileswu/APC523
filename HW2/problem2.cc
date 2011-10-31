#include "minimization.h"
#include <iostream>
#include <cmath>
#include <complex>

using namespace std;

double x2(double i) { return(i*i + 50*i); }
double x4(double i) { return(i*i*i*i + 5*i*i + 50*i); }

double v(Vector v) { return((v[0] - 1)*(v[0] - 1) + (v[1] - 1.5)*(v[1]-1.5)); }


double f(Vector v) {
	complex<double> e (0.5, 0);
	complex<double> one (1.0, 0);
	complex<double> w (v[0], v[1]);

	complex<double> retval;
	complex<double> wminus1squared = (w - one) * (w - one);
	retval = e*e*(one - w*w/e)*wminus1squared*wminus1squared - (one - wminus1squared)*w*w*w*w;

	return(retval.imag()*retval.imag() + retval.real()*retval.real());
}

int main() {
	//OneDFunctor f(sin);

	//brent(1.6, f);

	Vector x0(2);
	x0[0] = 0.4;
	x0[1] = 0.4;

	Vector line(2);
	line[0] = 1;
	line[1] = 1;
	//Vector out = minimize_along_line(x0, line, v);
	//out.print();
	powell(x0, f);
	cout << "Returned to main" << endl;
	
	return(0);
}



