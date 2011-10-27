#include <string>
using namespace std;

class Vector {

	private:
		double *data;
		int size;
	public:
		Vector (int n);
		~Vector ();
		Vector(const Vector& rhs);
		Vector& operator=(Vector& rhs);

		void print();
		string to_s();
		int dimensions();

		double& operator[] (int i);

};

Vector operator+(Vector& a, Vector& b);
Vector operator-(Vector& a, Vector& b);
Vector operator*(Vector& a, double b); 
