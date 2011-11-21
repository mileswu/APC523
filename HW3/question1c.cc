#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/time.h>
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

double deviation(double t, double y[], double y_good[]) {
	double e = 0;
	e += pow(y[1] - y_good[1], 2);
	e += pow(y[0] - y_good[0], 2);
	e = sqrt(e);
	return(e);
}

void output(ofstream &f, double t, double *y, double *y_good) {
	f << t << " ";
	f << y[0] << " " << y[1] << " ";
	f << energy(y) << " ";
	f << angularmomentum(y) << " ";
	f << deviation(t, y, y_good);

	f << endl;
}


void init(double *y) {
	/* Circular */
	y[0] = 1;
	y[1] = 0;
	y[2] = 0;
	y[3] = 1;

	/* Eliptical */
	y[3] = sqrt(19);
	y[0] = 0.1;
}

int main() {
	double y[4]; //0=x, 1=y, 2=v_x, 3=v_y
	double y_good[4];
	double dydt[4];
	double timestep = 0.0005;
	double max = 3.141592654*2*20;
	ofstream f;
	struct timeval tp, tp2;
	double time;

	gettimeofday(&tp, NULL);
	f.open("q1-euler.csv");
	init(y);
	init(y_good);
	int no_of_orbits = 0;
	double t2=0;
	for(double t=0; no_of_orbits<10  && t<max; t+=timestep) {
		double old_y = y[1];
		output(f, t, y, y_good);
		euler_iterate(t, y, 4, timestep, deriv_func);
		
		t2 = t;
		for(int i=0; i<200; i++) {
		deriv_func(t2, y_good, dydt);
		rk4(y_good-1, dydt-1, 4, t, timestep/200, y_good-1, deriv_offsetcorrected);
		}

		if(old_y < 0 && y[1] > 0)
			no_of_orbits++;
	}
	f.close();
	gettimeofday(&tp2, NULL);
	time = tp2.tv_sec - tp.tv_sec + ((double)(tp2.tv_usec - tp.tv_usec))/1000000.0;
	cout << "Euler: " << time << "s" << endl;	


	f.open("q1-rk4.csv");
	timestep *= 6.8;
	gettimeofday(&tp, NULL);
	init(y);
	init(y_good);
	for(double t=0; t<max; t+=timestep) {
		output(f, t, y, y_good);
		deriv_func(t, y, dydt);
		rk4(y-1, dydt-1, 4, t, timestep, y-1, deriv_offsetcorrected);
		double yscal[5] = {1, 1, 1, 1, 1};
		double eps = 1e-4;
		double timestep_todo;
		double timestep_did;
		t2 = t;
		for(int i=0; i<800; i++) {
		deriv_func(t2, y_good, dydt);
		rk4(y_good-1, dydt-1, 4, t, timestep/800, y_good-1, deriv_offsetcorrected);
		}
	}
	f.close();
	gettimeofday(&tp2, NULL);
	time = tp2.tv_sec - tp.tv_sec + ((double)(tp2.tv_usec - tp.tv_usec))/1000000.0;
	cout << "RK4: " << time << "s" << endl;	

	gettimeofday(&tp, NULL);	
	f.open("q1-bs.csv");
	timestep *= 25.0/6.8;
	init(y);
	double timestep_did;
	for(double t=0; t<max;) {
		deriv_func(t, y, dydt);
		output(f, t, y, y_good);
		double yscal[5] = {1, 1, 1, 1, 1};
		double eps = 1e-4;
		double timestep_todo;
		bsstep(y-1, dydt-1, 4, 
				&t,
				timestep, eps,
				yscal,
				&timestep_did, &timestep_todo,
				deriv_offsetcorrected);
		t2 = t;
		for(int i=0; i<2000; i++) {
		deriv_func(t2, y_good, dydt);
		rk4(y_good-1, dydt-1, 4, t, timestep/2000, y_good-1, deriv_offsetcorrected);
		}
	}
	f.close();
	gettimeofday(&tp2, NULL);
	time = tp2.tv_sec - tp.tv_sec + ((double)(tp2.tv_usec - tp.tv_usec))/1000000.0;
	cout << "BS: " << time << "s" << endl;	


	return 0;
}
