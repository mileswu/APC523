#pragma once

#define X 0
#define Y 1 
#define Z 2
#define K 3
#define LEAF 4

#define TRUE 1
#define FALSE 0

struct particle;

struct tree {
	// Info about this node
	double size;	
	struct particle *p;

	// Children (if applicable)
	int dimension;
	double boundary;
	struct tree *left;
	struct tree *right;
	struct tree *above;
};

typedef struct tree tree;

tree *build_tree(struct particle **ps, int size, int depth);
