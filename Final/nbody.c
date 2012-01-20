#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "particles.h"

particle *test_particle() {
	particle *ps = malloc(sizeof(particle)*4);
	ps[0].x = 0; ps[0].y = 1;
	ps[1].x = 1; ps[1].y = 2;
	ps[2].x = 2; ps[2].y = 0;
	ps[3].x = 3; ps[3].y = 4;
	
	return(ps);
}

int main() {
	particle *ps = test_particle();

	int i, size = 4;

	particle **tree_copy = malloc(sizeof(particle *)*size);
	for(i=0; i<size; i++) {
		tree_copy[i] = ps + i;
	}

	tree *root;
	root = build_tree(tree_copy, size, 0);

	output_image(ps, size, root);

	return(0);
}
