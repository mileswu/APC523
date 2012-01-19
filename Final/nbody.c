#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

particle *test_particle() {
	particle *ps = malloc(sizeof(particle)*4);
	ps[0].x = 0;
	ps[1].x = 1;
	ps[2].x = 2;
	ps[3].x = 3;
	
	return(ps);
}

int main() {
	particle *ps = test_particle();

	int i, size = 4;

	particle **tree_copy = malloc(sizeof(particle *)*size);
	for(i=0; i<size; i++) {
		tree_copy[i] = ps + i;
	}

	build_tree(tree_copy, size, 0);

	return(0);
}
