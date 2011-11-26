class Matrix {

	public:
		double *data;
		int cols, rows;
		Matrix (int, int);
		~Matrix ();
		Matrix(const Matrix& rhs);
		Matrix& operator=(Matrix& rhs);

		double& operator() (int r, int c);
};
