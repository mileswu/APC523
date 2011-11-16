#include <iostream>
#include <fstream>
#include <cmath>

#include <gsl/gsl_odeiv.h>

using namespace std;


void euler_iterate(double *x, double *y, double *v_x, double *v_y, double timestep) {
	double prev_v_x = *v_x;
	double prev_v_y = *v_y;

	*v_x -= 1.0 / pow(*x * *x + *y * *y, 1.5) * *x * timestep;
	*v_y -= 1.0 / pow(*x * *x + *y * *y, 1.5) * *y * timestep;

	*x += prev_v_x * timestep;
	*y += prev_v_y * timestep;
}

int gsl_func(double t, const double y[], double dydt[], void *params) {
	dydt[0] = y[2];
	dydt[1] = y[3];
	dydt[2] = -1.0 / pow(y[0] * y[0] + y[1] * y[1], 1.5) * y[0];
	dydt[3] = -1.0 / pow(y[0] * y[0] + y[1] * y[1], 1.5) * y[1];

	return 0;
}

void gsl_iterate(double *x, double *y, double *v_x, double *v_y, double timestep, const gsl_odeiv_step_type * t) {
	gsl_odeiv_system sys = {gsl_func, NULL, 4, NULL};
	gsl_odeiv_step *step = gsl_odeiv_step_alloc(t, 4);

	double yarr[4] = {*x, *y, *v_x, *v_y};
	yarr[0] = *x; yarr[1] = *y; yarr[2] = *v_x; yarr[3] = *v_y;
	double y_err[4];

	gsl_odeiv_step_apply(step, 0, timestep, yarr, y_err, NULL, NULL, &sys);

	*x = yarr[0];
	*y = yarr[1];
	*v_x = yarr[2];
	*v_y = yarr[3];

	gsl_odeiv_step_free(step);
}

void rk4_iterate(double *x, double *y, double *v_x, double *v_y, double timestep) {
	gsl_iterate(x, y, v_x, v_y, timestep, gsl_odeiv_step_rk4);
}

void output(ofstream *f, double t, double x, double y, double v_x, double v_y) {
	*f << t << " ";
	*f << x << " " << y << " ";
	*f << v_x << " " << v_y << " ";
	*f << endl;
}

int main() {
	double x=1, y=0;
	double v_x=0, v_y=1;
	double timestep = 0.01;
	
	ofstream f;
	f.open("q1-euler.csv");


	for(double t=0; t<60; t+=timestep) {
		euler_iterate(&x, &y, &v_x, &v_y, timestep);
		output(&f, t, x, y, v_x, v_y);
	}
	f.close();

	f.open("q1-rk4.csv");
	x=1; y=0; v_x=0; v_y=1;
	for(double t=0; t<60; t+=timestep) {
		rk4_iterate(&x, &y, &v_x, &v_y, timestep);
		output(&f, t, x, y, v_x, v_y);
	}
	
	f.close();


	return 0;
}
