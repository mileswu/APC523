
typedef struct {
	double x;
	double y;
	double z;

	double v_x;
	double v_y;
	double v_z;

	double mass;
} particle;

void output_image(particle *ps, int num_particles);
