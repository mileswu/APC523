#include "minimization.h"
#include <iostream>
#include <cmath>

using namespace std;

double x2(double i) { return(i*i + 50*i); }
double x4(double i) { return(i*i*i*i + 5*i*i + 50*i); }

int main() {
	brent(1.6, cos);

	return(0);
}


