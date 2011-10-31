#include "minimization.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <cmath>
#include <list>

using namespace std;

template <class C>
void bracket(double x0, double *ret_a, double *ret_c, C &f) {
	// Bracket a minimum given a functor f and a start value x0. Results are returned in ret_a and ret_c

	// Parameters that affect the bracketing. Smallstep is the starting step and ratio is the
	// multiplication factor that it does every time
	double smallstep = 1e-5;
	double ratio = 2;

	double a, b, c;

	// Have 'b' be the x0 provided and 'a' be back a bit by a smallstep
	a = x0 - smallstep;
	b = x0;

	// Swap so that we are orientated towards a downhill slope
	if(f(b) > f(a)) {
		swap(&a, &b);
	}

	// Arbitarly set 'c' so that 'b' is the midpoint
	c = b + (b-a); 

	int no_of_iters = 0;
	for(;;) {
		// We found a 'b' which is lower than 'a' and 'c'
		if(f(a) > f(b) && f(c) > f(b)) break;

		// Move everything down the slope. a->b, b->c, c moves by (c-b) * ratio
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

	// Swap them if they are the wrong way round
	if(a > c) {
	 	*ret_a = a;
		*ret_c = c;
	} else {
	 	*ret_c = a;
		*ret_a = c;
	}
	
	return;
}

void bracket(double x0, double *ret_a, double *ret_c, double(*f)(double)) {
	// Bracket a minimum given a function f and a start value x0. Results are returned in ret_a and ret_c
	OneDFunctor b(f);
	bracket(x0, ret_a, ret_c, b); 
}

void swap(double *a, double *b) {
	// Swaps values of a and b
	double temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

template <class C>
double choose_lowest_three(double *a, double *b, double *c, double *d, C &f) {
	// Choose the lowest three f(a/b/c) values and keep them in a,b,c spatial order
	// It also returns which one was best
	
	// Simple swap sort to order them in ascending value of f(), so we can choose first three to keep
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
	
	// Swap them back so that they are in ascending order of a, b, c 
	for(int i=0; i<3; i++) {
		if(*a > *b)
			swap(a, b);
		if(*b > *c)
			swap(b, c);
	}

	return(best);
}

template <class C>
double brent(double x0, C &f) {
	// Perform Brent's method to find a minimum given x0 and a functor f
	double a, b, c, d, prev_movement, prev_prev_movement, best;

	// Golden ratio used in Brent's
	double fraction = (3.0 - sqrt(5.0))/2.0;

	// Target precision
	double precision = 1e-5;

	// Bracket first
	bracket(x0, &a, &c, f);
	b = a + (c - a)/2.0;

	for(int iter=0;iter<1000;iter++) {
		//Try parabola
		double ba, fbfa, bc, fbfc;
		ba = b - a;
		fbfa = f(b) - f(a);
		bc = b - c;
		fbfc = f(b) - f(c);

		// This is the minimum for the parabola
		d = b - 0.5* (ba*ba*fbfc - bc*bc*fbfa) / (ba*fbfc - bc*fbfa);
		
		int parabola_success = 0;
		// d must fall within (a,c)
		if(d > a && d < c) {
			// d must imply movement from best x less than half the movement of the step before last
			if(iter < 2 || fabs(d - best) < 0.5*prev_prev_movement )
				parabola_success = 1;
		}

		// Parabola was not good enough. Revert to bisection and golden ratio
		if(parabola_success == 0) {
			if(b - a > c - b)
				d = a + fraction*(b-a);
			else
				d = b + fraction*(c-b);
		}

		DEBUG("d - " << d);
		DEBUG("parabola: " << parabola_success);

		// Chose lowest three
		best = choose_lowest_three(&a, &b, &c, &d, f);

		DEBUG("Fmin: "
		<< "f(A=" << a << ") = " << f(a) << ". "
		<< "f(B=" << b << ") = " << f(b) << ". "
		<< "f(C=" << c << ") = " << f(c) << ". ");

		// End check to see if it meets precision
		if(fabs(c-a) < precision) {
			DEBUG("Fmin final result: f(" << b << ") = " << f(b));
			DEBUG("after " << iter << " iterations");
			return(b);
		}

		// Store movements
		prev_prev_movement = prev_movement;
		prev_movement = fabs(d - best);
		
		DEBUG("--");
	}
	cerr << "Iterations exceeded. Seek help" << endl;
	exit(1);
}

double brent(double x0, double(*f)(double)) {
	// Perform Brent's method to find a minimum given x0 and a function f
	OneDFunctor b(f);
	return(brent(x0, b));
}

OneDFunctor::OneDFunctor(double(*f)(double)) {
	// Constructor for 1D functor
	function = f;
}

double OneDFunctor::operator() (double x) {
	// Operator for () call that runs function
	return(function(x));
}

NDto1DFunctor::NDto1DFunctor(Vector &x0_i, Vector &line_i, double(*f)(Vector)) : x0(3), line(3) {
	// Constructor for ND functor. This allows N-D functions to be used as if they were 1D by
	// varying their position as distance along a line from the start point.
	// Takes in start vector x0, line line_i, and function f
	x0 = x0_i;
	line = line_i;
	function = f;
}

double NDto1DFunctor::operator() (double l) {
	// Operator for () call that runs function at point along line
	Vector temp = line * l;
	return(function(x0 + temp));
}

Vector minimize_along_line(Vector &x0, Vector &line, double(*f)(Vector)) {
	// Minimize an N-dimensional function along a line. Needs start vector, line and function.
	
	// Create functor
	NDto1DFunctor functor(x0, line, f);
	double x;

	// Do Brent
	x = brent(0.0, functor);
	Vector temp = line * x;
	
	DEBUG("Minimized along line " << line.to_s() << ": f" << (x0 + temp).to_s() << "=" << functor(x));
	return(x0 + temp);
}

Vector powell(Vector &x0, double(*f)(Vector)) {
	// Minimize an N-dimensional function using Powell's method. Takes in function and start point x0.
	int dimensions = x0.dimensions();

	// List of search vectors and positions
	list<Vector> u;
	list<Vector> p;

	// Target precision
	double precision = 1e-5;
	double previous_value=DBL_MAX;

	// Initialize the search vectors to unit ones
	for(int i=0; i<dimensions; i++) {
		Vector temp(dimensions);
		temp[i] = 1.0;
		u.push_back(temp);
	}

	// Set p_0
	p.push_back(x0);

	for(int iter=0; iter<1000; iter++) {
		// Iterate through list of 'u's and minimize each p according to that search vector
		list<Vector>::iterator p_it = p.begin();
		for(list<Vector>::iterator u_it=u.begin(); u_it != u.end(); u_it++) {
			Vector temp = minimize_along_line(*p_it, *u_it, f);
			p.push_back(temp);
			p_it++;
		}

		// shift u's and insert u_n
		u.pop_front();
		u.push_back(p.back() - p.front());

		// Minimize p_n and store in p_0
		Vector temp = minimize_along_line(p.back(), u.back(), f);
		p.push_front(temp);

		// Drop all the old p's
		p.resize(1, dimensions);
		
		DEBUG("Powell iteration. f" << p.front().to_s() << "=" << f(p.front()));

		// Check result to see if it meets the precision goal
		if(fabs(f(p.front()) - previous_value) < precision) {
			DEBUG("Finished Powell. f" << p.front().to_s() << "=" << f(p.front()) << " in " << iter << "iterations");
			return(p.front());
		}
		previous_value = f(p.front());
	}
	cerr << "Iterations exceeded. Seek help" << endl;
	exit(1);
}


	




		

