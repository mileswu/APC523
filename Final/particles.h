struct tree;

struct particle {
	double x;
	double y;
	double z;

	double v_x;
	double v_y;
	double v_z;

	double a_x;
	double a_y;
	double a_z;

	double mass;
};
typedef struct particle particle;

void output_image(char *f, particle *ps, int num_particles, struct tree *root);
void randomize_particles(particle *ps, int size);
