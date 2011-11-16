void bsstep(double y[], double dydx[], int nv, double *xx, double htry, double eps,
        double yscal[], double *hdid, double *hnext,
        void (*derivs)(double, double [], double []));
