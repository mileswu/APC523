#include <stdio.h>
#define _GNU_SOURCE
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "particles.h"

// BSD: has arg before a,b
int median_qsort(void *a, void *b, void *dimension) {
	int d = *((int *)dimension);
	double val1, val2;
	
	if(d == X) {
		val1 = (*(particle **)a)->x;
		val2 = (*(particle **)b)->x;
	} else if (d == Y) {
		val1 = (*(particle **)a)->y;
		val2 = (*(particle **)b)->y;
	} else if (d == Z) {
		val1 = (*(particle **)a)->z;
		val2 = (*(particle **)b)->z;
	} else {
		assert(1 == 0);
	}

	if(val1 > val2)
		return 1;
	else if(val1 == val2)
		return 0;
	else
		return -1;
}

double median(particle **ps, int size, int dimension) {
	// GNU only, comparator last
	qsort_r(ps, size, sizeof(particle *), median_qsort, &dimension);

	particle *p1 = ps[size/2 - 1];
	particle *p2 = ps[size/2];
	
	if(dimension == X)
		return 0.5*(p1->x + p2->x);
	else if (dimension == Y)
		return 0.5*(p1->y + p2->y);
	else if (dimension == Z)
		return 0.5*(p1->z + p2->z);
	else
		assert(1 == 0);
}

tree *build_tree(particle **ps, int size, int depth) {
	tree *t = malloc(sizeof(tree));
	if(size == 1) {
		t->p = ps[0];
		t->dimension = LEAF;
		return t;
	}

	int dimension = depth%K;
	// this qsorts too
	double med = median(ps, size, dimension);
	
	particle **left, **right;
	left = ps;
	right = ps + size/2;

	// Tree building
	t->dimension = dimension;
	t->boundary = med;
	t->left = build_tree(left, size/2, depth+1);
	t->right = build_tree(right, size/2, depth+1);
	t->left->above = t;
	t->right->above = t;

	// Virtual particle
	t->p = malloc(sizeof(particle));
	int i;
	t->p->x = 0; t->p->y = 0; t->p->z = 0;
	for(i=0; i<size; i++) {
		t->p->x += ps[i]->x*ps[i]->mass;
		t->p->y += ps[i]->y*ps[i]->mass;
		t->p->y += ps[i]->y*ps[i]->mass;
		t->p->mass += ps[i]->mass;
	}
	t->p->x /= t->p->mass;
	t->p->y /= t->p->mass;
	t->p->z /= t->p->mass;

	double max_size = 0;
	for(i=0; i<size; i++) {
		double r = (ps[i]->x - t->p->x)*(ps[i]->x - t->p->x) + (ps[i]->y - t->p->y)*(ps[i]->y - t->p->y) + (ps[i]->z - t->p->z)*(ps[i]->z - t->p->z);
		if(r > max_size)
			max_size = r;
	}
	t->size = sqrt(max_size);

	return(t);
}
