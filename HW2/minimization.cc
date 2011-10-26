#include "minimization.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

using namespace std;

void bracket(double x0, double *ret_a, double *ret_c, double(*f)(double)) {
	/* What if x0 is a global maximum then this will fail */
	double smallstep = DBL_EPSILON;
	double ratio = 2;
	

	double a, b, c;
	a = x0 - smallstep;
	b = x0;
	if(f(b) > f(a)) {
		double temp;
		temp = b;
		b = a;
		a = temp;
	}
	c = b + (b-a); 

	int no_of_iters = 0;
	for(;;) {
		if(f(a) > f(b) && f(c) > f(b)) break;

		double temp;
		temp = c-b;
		a = b;
		b = c;
		c += temp*ratio;

		no_of_iters++;
		if(no_of_iters > 1000) {
			cout << "Iterations exceeded. Seek help" << endl;
			cout << "Current a: " << a << " c: " << c << endl;
			exit(1);
		}
	}

	cout << "Bracketing (x0 = " << x0 << "): ";
	cout << "f(A=" << a << ") = " << f(a) << ". ";
	cout << "f(B=" << b << ") = " << f(b) << ". ";
	cout << "f(C=" << c << ") = " << f(c) << ". " << endl;
	cout << "Found after " << no_of_iters << " iterations." << endl;

	*ret_a = a;
	*ret_c = c;
	return;
}
