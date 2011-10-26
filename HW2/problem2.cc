#include "minimization.h"
#include <iostream>

using namespace std;

double x2(double i) { return(i*i + 50*i); }

int main() {
	double ret_a, ret_c;
	bracket(1.0, &ret_a, &ret_c, x2);

	cout << "A: " << ret_a << " C: " << ret_c << endl;
	

	return(0);
}


