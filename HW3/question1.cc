#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

#include "numericalrecipes/rk4.h"
#include "numericalrecipes/bsstep.h"

void euler_iterate(double *x, double *y, double *v_x, double *v_y, double timestep) {
	double prev_v_x = *v_x;
	double prev_v_y = *v_y;

	*v_x -= 1.0 / pow(*x * *x + *y * *y, 1.5) * *x * timestep;
	*v_y -= 1.0 / pow(*x * *x + *y * *y, 1.5) * *y * timestep;

	*x += prev_v_x * timestep;
	*y += prev_v_y * timestep;
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

void output(ofstream &f, double t, double *y) {
	f << t << " ";
	f << y[0] << " " << y[1] << " ";
	f << y[2] << " " << y[3] << " ";
	f << endl;
}

int main() {
	double y[4] = {1, 0, 0, 1}; //0=x, 1=y, 2=v_x, 3=v_y
	double dydt[4];
	double timestep = 0.01;
	
	ofstream f;
	//f.open("q1-euler.csv");

	/*for(double t=0; t<60; t+=timestep) {
		euler_iterate(&x, &y, &v_x, &v_y, timestep);
		output(&f, t, x, y, v_x, v_y);
	}
	f.close();*/

	f.open("q1-rk4.csv");
	for(double t=0; t<10; t+=timestep) {
		deriv_func(t, y, dydt);
		output(f, t, y);
		rk4(y-1, dydt-1, 4, t, timestep, y-1, deriv_offsetcorrected);
	}
	f.close();
	
	f.open("q1-bs.csv");
	timestep = 0.01;
	double timestep_did;
	for(double t=0; t<10;) {
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
		cout << "timestep: " << timestep_did << endl;
	}
	f.close();


	return 0;
}
