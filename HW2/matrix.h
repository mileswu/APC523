class Matrix {

	public:
		double *data;
		int cols, rows;
		Matrix (int, int);
		~Matrix ();

		double& operator() (int r, int c);
};
