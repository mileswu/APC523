#include "particles.h"

#define X 1
#define Y 2 
#define Z 3
#define LEAF 0

#define TRUE 1
#define FALSE 0

struct tree {
	// Info about this node
	
	particle *p;

	// Children (if applicable)
	int dimension;
	double boundary;
	struct tree *left;
	struct tree *right;
};

typedef struct tree tree;


