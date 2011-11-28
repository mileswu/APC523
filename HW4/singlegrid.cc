#include "jacobi.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;
#define N 255

int main(int argv, char **argc) {
	// Argument parsing for no of iterations
	int iter_num;
	if(argv != 2) {
		cout << "Usage: " << argc[0] << " <no of iterations>" << endl;
		return(1);
	}
	iter_num = atoi(argc[1]);
	if(iter_num < 1) {
		cout << "Invalid argument for no of iterations" << endl;
		cout << "Usage: " << argc[0] << " <no of iterations>" << endl;
		return(1);
	}

	// Creation of all the matrix and vectors
	Matrix matrix = finitediff_matrix(N+2);
	Matrix f = f_vector(N+2);
	Matrix u = u_vector(N+2);
	
	// Run
	for(int i=0; i<iter_num; i++) {
		jacobi_iteration(matrix, u, f);
		if(i%100 == 0 && iter_num > 100)
			cout << "Iteration: " << i << "/" << iter_num << endl;
	}
	
	// Open output file for the data points (to be used with plotting)
	ofstream fout;
	stringstream ss;
	ss << "singlegrid_n_" << iter_num << ".csv";
	cout << "Writing to: " << ss.str().c_str() << endl;
	fout.open(ss.str().c_str());

	for(int i=0; i<N+2; i++) {
		double x = 1.0/(N+1) * i;
		fout << x << " " << u(i, 0) << endl;
	}

	fout.close();

	return(0);
}
