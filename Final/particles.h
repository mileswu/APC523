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
	int id;
	int color;
};
typedef struct particle particle;

void output_image(char *f, particle *ps, int num_particles, struct tree *root, double range);
void galaxy(particle *ps, int size, double xoffset, double yoffset, double xvel, int color);
void randomize_uniform_sphere(particle *ps, int size);
