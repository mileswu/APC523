#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

#define END 1.0
#define START 0.0
#define A 1.0
#define PI M_PI

double getx(int i, int n) {
		double x = START + (END-START)/((double)n+1.0) * i;
		return x;
}

void lf(vector<double> &grid, int n, double cfl) {
	vector<double> oldgrid(grid);

	grid[0] += 0.5*(oldgrid[n] - 2.0*oldgrid[0] + oldgrid[1]) - 0.5*cfl*(oldgrid[1] - oldgrid[n]);
	grid[n+1] = grid[0];

	for(int i=1; i<n+1; i++) {
		grid[i] += 0.5*(oldgrid[i-1] - 2.0*oldgrid[i] + oldgrid[i+1]) - 0.5*cfl*(oldgrid[i+1] - oldgrid[i-1]);
	}
}

void upwind(vector<double> &grid, int n, double cfl) {
	vector<double> oldgrid(grid);

	grid[0] += 0.5*cfl*(oldgrid[n] - 2.0*oldgrid[0] + oldgrid[1]) - 0.5*cfl*(oldgrid[1] - oldgrid[n]);
	grid[n+1] = grid[0];

	for(int i=1; i<n+1; i++) {
		grid[i] += 0.5*cfl*(oldgrid[i-1] - 2.0*oldgrid[i] + oldgrid[i+1]) - 0.5*cfl*(oldgrid[i+1] - oldgrid[i-1]);
	}
}

void lw(vector<double> &grid, int n, double cfl) {
	vector<double> oldgrid(grid);

	grid[0] = oldgrid[0] + 0.5*cfl*cfl*(oldgrid[n] -2.0*oldgrid[0] + oldgrid[1]) - 0.5*cfl*(oldgrid[1] - oldgrid[n]);
	grid[n+1] = grid[0];

	for(int i=1; i<n+1; i++) {
		grid[i] = oldgrid[i] + 0.5*cfl*cfl*(oldgrid[i-1] - 2.0*oldgrid[i] + oldgrid[i+1]) - 0.5*cfl*(oldgrid[i+1] - oldgrid[i-1]);
	}
}

void init(vector<double> &grid, int n) {
	for(int i=0; i<n+2; i++) {
		double x = getx(i, n);
		grid[i] = sin(2.0*PI*x);
	}
}

double l1error(vector<double> &grid, vector<double> &grid_t0) {
	int n = grid.size();
	double l1=0;
	for(int i=0; i<n; i++) {
		l1 += fabs(grid[i] - grid_t0[i]) / n ;///fabs(grid[i]);
	}
	return l1;
}

int main() {
	double cfl = 0.5;

	for(int n=16; n<=1024; n*=2) {
		double dx = (END-START)/((double)n+1.0);
		double timestep = cfl*dx/A;
		double t;	

		double l1_lf, l1_lw, l1_upwind;
		vector<double> grid(n+2);
		vector<double> grid_t0(n+2);
		init(grid_t0, n);

		init(grid, n);
		for(t=timestep; t < 1; t += timestep)
			lf(grid, n, cfl);
		
		t-=timestep;
		double leftover = 1.0 - t;
		double tempcfl = A*leftover/dx;
		
		lf(grid, n, tempcfl);

		l1_lf = l1error(grid, grid_t0);

		
		init(grid, n);
		for(t=timestep; t < 1; t += timestep)
			upwind(grid, n, cfl);
		upwind(grid, n, tempcfl);
		l1_upwind = l1error(grid, grid_t0);

		init(grid, n);
		for(t=timestep; t < 1; t += timestep) {
			lw(grid, n, cfl);
		}
		lw(grid, n, tempcfl);
	
		l1_lw = l1error(grid, grid_t0);

		ofstream f;
		f.open("q2a.csv");
		for(int j=0; j<n+2; j++)
			f << getx(j, n) << " " << grid_t0[j] << endl;
		f.close();

		cout << n << " " << l1_lf << " " << l1_upwind << " " << l1_lw << endl;
	}


	return 0;
}
