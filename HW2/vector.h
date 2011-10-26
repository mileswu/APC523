class Vector {
	private:
		double *data;
		int size;
	public:
		Vector (int n);
		~Vector ();

		void print();
		int dimensions();

		double& operator[] (int i);

};

Vector operator+(Vector& a, Vector& b);
Vector operator*(Vector& a, double b); 
