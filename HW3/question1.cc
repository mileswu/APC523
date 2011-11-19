#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#include "numericalrecipes/rk4.h"
#include "numericalrecipes/bsstep.h"

void euler_iterate(double t, double *y, int n, double timestep, void (*deriv)(double, double *, double *)) {
	double dydt[n];
	deriv(t, y, dydt);

	for(int i=0; i<n; i++) {
		y[i] += dydt[i] * timestep;
	}
}

void deriv_func(double t, double y[], double dydt[]) {
	dydt[0] = y[2];
	dydt[1] = y[3];
	dydt[2] = -1.0 / pow(y[0] * y[0] + y[1] * y[1], 1.5) * y[0];
	dydt[3] = -1.0 / pow(y[0] * y[0] + y[1] * y[1], 1.5) * y[1];
}

void deriv_offsetcorrected(double t, double y[], double dydt[]) { 
	deriv_func(t, y+1, dydt+1); 
}

double angularmomentum(double y[]) {
	double l = 0;
	l += y[3]*y[0];
	l += -y[2]*y[1];
	return l;
}

double energy(double y[]) { 
	double energy = 0;
	energy += 0.5 * y[2]*y[2] + 0.5 * y[3]*y[3];
	energy -= 1.0 / sqrt(y[0]*y[0] + y[1]*y[1]);
	return energy;
}

void output(ofstream &f, double t, double *y) {
	f << t << " ";
	f << y[0] << " " << y[1] << " ";
	f << energy(y) << " ";
	f << angularmomentum(y);

	f << endl;
}


void init(double *y) {
	/* Circular */
	y[0] = 1;
	y[1] = 0;
	y[2] = 0;
	y[3] = 1;

	/* Eliptical */
}

int main() {
	double y[4]; //0=x, 1=y, 2=v_x, 3=v_y
	double dydt[4];
	double timestep = 0.3;
	double max = 3.141592654*2*1000;
	
	ofstream f;
	f.open("q1-euler.csv");
	init(y);
	for(double t=0; t<max; t+=timestep) {
		output(f, t, y);
		euler_iterate(t, y, 4, timestep, deriv_func);
	}
	f.close();

	f.open("q1-rk4.csv");
	init(y);
	for(double t=0; t<max; t+=timestep) {
		output(f, t, y);
		deriv_func(t, y, dydt);
		rk4(y-1, dydt-1, 4, t, timestep, y-1, deriv_offsetcorrected);
	}
	f.close();
	
	f.open("q1-bs.csv");
	init(y);
	double timestep_did;
	for(double t=0; t<max;) {
		deriv_func(t, y, dydt);
		output(f, t, y);
		double yscal[5] = {1, 1, 1, 1, 1};
		double eps = 1e-4;
		double timestep_todo;
		bsstep(y-1, dydt-1, 4, 
				&t,
				timestep, eps,
				yscal,
				&timestep_did, &timestep_todo,
				deriv_offsetcorrected);
	}
	f.close();


	return 0;
}
