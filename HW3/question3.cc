#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <sstream>
#include <vector>

#define M 1
#define G 1
#define epsilon 0.1

using namespace std;

void leapfrog_iterate(vector<double> &x, vector<double> &y, vector<double> &z, vector<double> &v_x, vector<double> &v_y,vector<double> &v_z, double timestep) {
	int num = x.size();
	
	for(int i=0; i<num; i++) {
		for(int j=0; j<num; j++) {
			if(i == j) continue;
			double r2 = pow(x[j] - x[i], 2) + pow(y[j] - y[i], 2) + pow(z[j] - z[i], 2);
			double denom = (r2 + epsilon*epsilon)*sqrt(r2);
			v_x[i] += G*M*M / denom * (x[j] - x[i]) * timestep;
			v_y[i] += G*M*M / denom * (y[j] - y[i]) * timestep;
			v_z[i] += G*M*M / denom * (z[j] - z[i]) * timestep;
		}
	}
	for(int i=0; i<num; i++) {
		x[i] += v_x[i] * timestep;
		y[i] += v_y[i] * timestep;
		z[i] += v_z[i] * timestep;
	}
}

void generate_random_point_in_sphere(double *x, double *y, double *z) {
	for(int i=0; i<10000; i++) {
		*x = ((double)rand())/RAND_MAX;
		if(rand()%2 == 0)
			*x *= -1;
		*y = ((double)rand())/RAND_MAX;
		if(rand()%2 == 0)
			*y *= -1;
		*z = ((double)rand())/RAND_MAX;
		if(rand()%2 == 0)
			*z *= -1;

		if(*x * *x + *y * *y + *z * *z <= 1)
			return;
	}
	cout << "Problem generating random point in sphere" << endl;
	exit(1);
}

double energy(vector<double> &x, vector<double> &y, vector<double> &z, vector<double> &v_x, vector<double> &v_y,vector<double> &v_z) {
	int num = x.size();
	double energy = 0;
	
	for(int i=0; i<num; i++) {
		energy += 0.5 * M * (v_x[i]*v_x[i] + v_y[i]*v_y[i] + v_z[i]*v_z[i]);
	}
	for(int i=0; i<num; i++) {
		for(int j=i+1; j<num; j++) {
			energy -= G * M * M / sqrt(pow(x[j] - x[i], 2) + pow(y[j] - y[i], 2) + pow(z[j] - z[i], 2) + epsilon*epsilon);
		}
	}

	return energy;
}

int main(int argc, char **argv) {
	int no = 800;
	double timestep;
	vector<double> x(no);
	vector<double> y(no);
	vector<double> z(no);
	vector<double> v_x(no);
	vector<double> v_y(no);
	vector<double> v_z(no);
	
	timestep = 0.0001;
	srand(31231);

	for(int i=0; i<no; i++) {
		generate_random_point_in_sphere(&x[i], &y[i], &z[i]);
		v_x[i] = 0;
		v_y[i] = 0;
		v_z[i] = 0;
	}


	ofstream f, energy_f;
	//energy_f.open("q3/q3-energy.csv");
	int n = 0;
	for(double t=0; t<=10; t+=timestep) {
		/*stringstream ss;
		ss << "q3/q3-" << n << ".csv";
		f.open(ss.str().c_str());

		for(int i=0; i<no; i++) {
			f << x[i] << " " << y[i] << " " << z[i] << endl;
		}
		energy_f << t << " " << energy(x, y, z, v_x, v_y, v_z) << endl;
		f.close();*/

		leapfrog_iterate(x, y, z, v_x, v_y, v_z, timestep);
		n++;
	}
	//energy_f.close();



	return 0;

}

