#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define N 100
#define END 2.0
#define START -2.0
#define CFL 0.8
#define A 1.0

double getx(int i) {
		double x = START + (END-START)/((double)N+1.0) * i;
		return x;
}

void lf(vector<double> &grid) {
	vector<double> oldgrid(grid);

	grid[0] = 0.5*(oldgrid[N] + oldgrid[1]) - 0.5*CFL*(oldgrid[1] - oldgrid[N]);
	grid[N+1] = grid[0];

	for(int i=1; i<N+1; i++) {
		grid[i] = 0.5*(oldgrid[i-1] + oldgrid[i+1]) - 0.5*CFL*(oldgrid[i+1] - oldgrid[i-1]);
	}
}

void upwind(vector<double> &grid) {
	vector<double> oldgrid(grid);

	grid[0] = 0.5*CFL*(oldgrid[N] + oldgrid[1]) - 0.5*CFL*(oldgrid[1] - oldgrid[N]);
	grid[N+1] = grid[0];

	for(int i=1; i<N+1; i++) {
		grid[i] += 0.5*CFL*(oldgrid[i-1] - 2.0*oldgrid[i] + oldgrid[i+1]) - 0.5*CFL*(oldgrid[i+1] - oldgrid[i-1]);
	}
}

void lw(vector<double> &grid) {
	vector<double> oldgrid(grid);

	grid[0] = 0.5*CFL*CFL*(oldgrid[N] + oldgrid[1]) - 0.5*CFL*(oldgrid[1] - oldgrid[N]);
	grid[N+1] = grid[0];

	for(int i=1; i<N+1; i++) {
		grid[i] += 0.5*CFL*CFL*(oldgrid[i-1] - 2.0*oldgrid[i] + oldgrid[i+1]) - 0.5*CFL*(oldgrid[i+1] - oldgrid[i-1]);
	}
}

void init(vector<double> &grid) {
	for(int i=0; i<N+2; i++) {
		double x = getx(i);

		if(x < 1 && x > -1)
			grid[i] = 1.0;
		else
			grid[i] = 0.0;
	}
}

void output(vector<double> &grid, const char f[]) {
	ofstream fout;
	fout.open(f);
	for(int i=0; i<N+2; i++) {
		fout << getx(i) << " " << grid[i] << endl;
	}

	fout.close();
}

int main() {
	vector<double> grid(N+2);
	
	init(grid);
	for(double t=0; t < 20; t += CFL*((END-START)/((double)N+1.0)/A))
		lf(grid);
	output(grid, "lf.csv");
	
	init(grid);
	for(double t=0; t < 20; t += CFL*((END-START)/((double)N+1.0)/A))
		upwind(grid);
	output(grid, "upwind.csv");

	init(grid);
	for(double t=0; t < 20; t += CFL*((END-START)/((double)N+1.0)/A))
		lw(grid);
	output(grid, "lw.csv");


	





	




	return 0;
}
