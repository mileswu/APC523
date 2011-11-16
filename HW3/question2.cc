#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <sstream>

#define M 1
#define OMEGA 1

using namespace std;

void leapfrog_iterate(double *x, double *v, double timestep) {
	*v += - OMEGA*OMEGA/M * *x * timestep;
	*x += *v * timestep;
}

int main(int argc, char **argv) {
	if(argc != 2) {
		cout << "Usage: " << argv[0] << " <timestep>" << endl;
		return 1;
	}

	double x, timestep, v;

	x = 1;
	timestep = atof(argv[1])/OMEGA;
	v = 0 + OMEGA*OMEGA/M*x * timestep/2 ;

	ofstream f;
	stringstream ss;
	ss << "q2-timestep-" << argv[1] << ".csv";
	f.open(ss.str().c_str());
	cout << "Writing to: " << ss.str() << endl;
	for(double t=0; t<=10; t+=timestep) {
		f << t << " " << x << endl;
		leapfrog_iterate(&x, &v, timestep);
	}
	f.close();



	return 0;

}

