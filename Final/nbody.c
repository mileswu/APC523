#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "particles.h"

particle *test_particle() {
	particle *ps = malloc(sizeof(particle)*4);
	ps[0].x = -1.7; ps[0].y = 0;
	ps[1].x = -0.2; ps[1].y = 1;
	ps[2].x = 0.4; ps[2].y = 1;
	ps[3].x = 1.0; ps[3].y = -1.7;
	
	return(ps);
}

int main() {
	//particle *ps = test_particle();
	
	int size = 512;
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
	root = build_tree(tree_copy, size, 0);

	output_image(ps, size, root);

	return(0);
}
