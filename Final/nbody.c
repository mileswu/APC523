#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "particles.h"

#define G 1
#define THETA 0.8

int calc_counter;

particle *test_particle() {
	particle *ps = malloc(sizeof(particle)*4);
	ps[0].x = -1.7; ps[0].y = 0;
	ps[1].x = -0.2; ps[1].y = 1;
	ps[2].x = 0.4; ps[2].y = 1;
	ps[3].x = 1.0; ps[3].y = -1.7;
	
	return(ps);
}

void calc_a_traverse(particle *p, tree *node) {
	double r2 = (p->x - node->p->x)*(p->x - node->p->x) + (p->y - node->p->y)*(p->y - node->p->y) + (p->z - node->p->z)*(p->z - node->p->z);

	if(node->dimension != LEAF) {
		if(node->size / sqrt(r2) > THETA) { //too close
			calc_a_traverse(p, node->left);
			calc_a_traverse(p, node->right);
			return;
		}
	} else { //IS LEAF
		if(p == node->p)
			return;
	}



	double common = G * node->p->mass / pow(r2, 1.5);
	p->a_x += common*(node->p->x - p->x);
	p->a_y += common*(node->p->y - p->y);
	p->a_z += common*(node->p->z - p->z);
	
	
	/*if(p->id == 529) {
		printf("size %f, r %f, mass %f\n", node->size, sqrt(r2), node->p->mass);
		printf("a_x %f a_y %f a_z %f\n", p->a_x, p->a_y, p->a_z);
	}*/

	calc_counter++;
}

void calc_a(particle *ps, int size, tree *root) {
	int i;
	#pragma omp parallel for private(i)
	for(i=0; i<size; i++) {
		ps[i].a_x = 0;
		ps[i].a_y = 0;
		ps[i].a_z = 0;
		calc_a_traverse(&ps[i], root);
	}
}

void iterate(double timestep, double *t, particle *ps, int size, tree *root) {
	calc_a(ps, size, root);
	int i;
	// Euler
	for(i=0; i<size; i++) {
		ps[i].v_x += ps[i].a_x * timestep;
		ps[i].v_y += ps[i].a_y * timestep;
		ps[i].v_z += ps[i].a_z * timestep;

		/*if(ps[i].a_x > 100000) {
			printf("Errr not good (id %d)\n", ps[i].id);
		}*/

		ps[i].x += ps[i].v_x * timestep;
		ps[i].y += ps[i].v_y * timestep;
		ps[i].z += ps[i].v_z * timestep;
	}
	*t += timestep;
}



int main() {
	srand(123);
	//particle *ps = test_particle();
	
	int size = 1024*2*2*2;
	size =4096*2*2;
	printf("Size - %d\n", size);

	particle *ps = malloc(sizeof(particle)*size);
	randomize_particles(ps, size);

	int i;

	/*for(i=0; i<size; i++) {
		printf("%f %f %f\n", ps[i].x, ps[i].y, ps[i].z);
	}*/

	particle **tree_copy = malloc(sizeof(particle *)*size);
	for(i=0; i<size; i++) {
		tree_copy[i] = ps + i;
	}

	tree *root;

	double t = 0, timestep = 0.0005, t_max = 0.03;
	timestep = 0.00005;
	int counter = 0;
	char *output_filename = malloc(sizeof(char)*1000);

	printf("Ready\n");
	while(t<t_max) {
		struct timeval tv1, tv2, tv3, tv4;
		gettimeofday(&tv1, NULL);

		
		sprintf(output_filename, "out-%06d.png", counter);
		output_image(output_filename, ps, size, NULL);
	
		gettimeofday(&tv2, NULL);
		

		root = build_tree(tree_copy, size, 0);
		gettimeofday(&tv3, NULL);
		printf("Tree built\n");

		calc_counter = 0;
		iterate(timestep, &t, ps, size, root);
		gettimeofday(&tv4, NULL);

		free_tree(root);
		counter++;
		
		double dt1 = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)/(double)1000000;
		double dt2 = (tv3.tv_sec - tv2.tv_sec) + (tv3.tv_usec - tv2.tv_usec)/(double)1000000;
		double dt3 = (tv4.tv_sec - tv3.tv_sec) + (tv4.tv_usec - tv3.tv_usec)/(double)1000000;
		printf("t - %f, calc eff:(%d)%f [o:%f t:%f c:%f]\n", t, calc_counter, (double)calc_counter/(double)(size*size), dt1, dt2, dt3);

	}

	free(output_filename);
	free(tree_copy);
	free(ps);


	return(0);
}
