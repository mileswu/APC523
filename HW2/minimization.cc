#include "minimization.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <cmath>

using namespace std;

void bracket(double x0, double *ret_a, double *ret_c, double(*f)(double)) {
	/* What if x0 is a global maximum then this will fail.
	 * Should also add in quadratic parabola stuff 
	 * */
	double smallstep = 1e-6;
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
			cerr << "Iterations exceeded. Seek help" << endl;
			cerr << "Current a: " << a << " c: " << c << endl;
			exit(1);
		}
	}

	DEBUG("Bracketing (x0 = " << x0 << "): "
	<< "f(A=" << a << ") = " << f(a) << ". "
	<< "f(B=" << b << ") = " << f(b) << ". "
	<< "f(C=" << c << ") = " << f(c) << ". "
	<< "Found after " << no_of_iters << " iterations.");

	if(a > c) {
	 	*ret_a = a;
		*ret_c = c;
	} else {
	 	*ret_c = a;
		*ret_a = c;
	}
	
	return;
}

void swap(double *a, double *b) {
	double temp;
	temp = *a;
	*a = *b;
	*b = temp;
}


double choose_lowest_three(double *a, double *b, double *c, double *d, double(*f)(double)) {
	double best;
	for(int i=0; i<4; i++) {
		if(f(*a) > f(*b))
			swap(a, b);
		if(f(*b) > f(*c))
			swap(b, c);
		if(f(*c) > f(*d))
			swap(c, d);
	}
	best = *a;
		DEBUG("choose lowest three a: "
		<< "f(A=" << *a << ") = " << f(*a) << ". "
		<< "f(B=" << *b << ") = " << f(*b) << ". "
		<< "f(C=" << *c << ") = " << f(*c) << ". "
		<< "f(D=" << *d << ") = " << f(*d) << ". ");
	
	for(int i=0; i<3; i++) {
		if(*a > *b)
			swap(a, b);
		if(*b > *c)
			swap(b, c);
	}
	return(best);
}

double brent(double x0, double(*f)(double)) {
	double a, b, c, d, prev_movement, prev_prev_movement, best;
	double fraction = (3.0 - sqrt(5.0))/2.0;
	double precision = 1e-6;
	//precision = 0.1;

	bracket(x0, &a, &c, f);
	b = a + (c - a)/2.0;

	for(int iter=0;iter<1000;iter++) {
		//Parabola
		double ba, fbfa, bc, fbfc;
		ba = b - a;
		fbfa = f(b) - f(a);
		bc = b - c;
		fbfc = f(b) - f(c);

		d = b - 0.5* (ba*ba*fbfc - bc*bc*fbfa) / (ba*fbfc - bc*fbfa);
		
		int parabola_success = 0;
		// d must fall within (a,c)
		if(d > a && d < c) {
			// imply movement from best x less than half the movement of the step before last
			if(iter < 2 || fabs(d - best) < 0.5*prev_prev_movement )
				parabola_success = 1;
		}


		if(parabola_success == 0) {
			// Bisection
			if(b - a > c - b)
				d = a + fraction*(b-a);
			else
				d = b + fraction*(c-b);
		}
		DEBUG("d - " << d);
		DEBUG("parabola: " << parabola_success);

		best = choose_lowest_three(&a, &b, &c, &d, f);

		DEBUG("Fmin: "
		<< "f(A=" << a << ") = " << f(a) << ". "
		<< "f(B=" << b << ") = " << f(b) << ". "
		<< "f(C=" << c << ") = " << f(c) << ". ");

		if(fabs(c-a) < precision) {
			DEBUG("Fmin final result: f(" << b << ") = " << f(b));
			DEBUG("after " << iter << " iterations");
			return(b);
		}

		prev_prev_movement = prev_movement;
		prev_movement = fabs(d - best);
		DEBUG("--");
	}
	cerr << "Iterations exceeded. Seek help" << endl;
	exit(1);
}


		

