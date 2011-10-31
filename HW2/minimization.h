#define DEBUG(str) do { std::cout << str << std::endl; } while( false )
//#define DEBUG(str) do { } while ( false )
//
#include "vector.h"

template <class C>
void bracket(double x0, double *a, double *b, C &f);

template <class C>
double brent(double x0, C &f);
Vector minimize_along_line(Vector &x0, Vector &line, double(*f)(Vector));
Vector powell(Vector &x0, double(*f)(Vector));

class NDto1DFunctor {
	public:
		double(*function)(Vector);
		Vector x0;
		Vector line;

		NDto1DFunctor(Vector &x0, Vector &line, double(*function)(Vector));

		double operator() (double i);
};

class OneDFunctor {
	public:
		double (*function)(double);
		OneDFunctor(double(*f)(double));
		double operator() (double x);
};
