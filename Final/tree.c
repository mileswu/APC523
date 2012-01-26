#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "tree.h"
#include "particles.h"

int median_qsortx(const void *a, const void *b) {
	double val1, val2;
	
	val1 = (*(particle **)a)->x;
	val2 = (*(particle **)b)->x;

	if(val1 > val2)
		return 1;
	else if(val1 == val2)
		return 0;
	else
		return -1;
}

int median_qsorty(const void *a, const void *b) {
	double val1, val2;
	
	val1 = (*(particle **)a)->y;
	val2 = (*(particle **)b)->y;

	if(val1 > val2)
		return 1;
	else if(val1 == val2)
		return 0;
	else
		return -1;
}

int median_qsortz(const void *a, const void *b) {
	double val1, val2;
	
	val1 = (*(particle **)a)->z;
	val2 = (*(particle **)b)->z;

	if(val1 > val2)
		return 1;
	else if(val1 == val2)
		return 0;
	else
		return -1;
}

double median(particle **ps, int size, int dimension) {
	if(dimension == X)
		qsort(ps, size, sizeof(particle *), median_qsortx);
	else if (dimension == Y)
		qsort(ps, size, sizeof(particle *), median_qsorty);
	else if (dimension == Z)
		qsort(ps, size, sizeof(particle *), median_qsortz);
	else
		assert(1 == 0);

	particle *p1 = ps[size/2 - 1];
	particle *p2 = ps[size/2];
	
	if(dimension == X)
		return 0.5*(p1->x + p2->x);
	else if (dimension == Y)
		return 0.5*(p1->y + p2->y);
	else if (dimension == Z)
		return 0.5*(p1->z + p2->z);
	return 0;
}

int depth(tree *t) {
	int d = 0;
	tree *t2 = t->above;
	while(t2!=NULL) {
		t2 = t2->above;
		d++;
	}

	return d;
}

tree *build_tree(particle **ps, int size, int depth) {
	tree *t = malloc(sizeof(tree));
	if(size == 1) {
		t->left = NULL;
		t->right = NULL;
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
	t->p->x = 0; t->p->y = 0; t->p->z = 0; t->p->mass = 0;
	for(i=0; i<size; i++) {
		t->p->x += ps[i]->x*ps[i]->mass;
		t->p->y += ps[i]->y*ps[i]->mass;
		t->p->z += ps[i]->z*ps[i]->mass;
		t->p->mass += ps[i]->mass;
	}
	t->p->x /= t->p->mass;
	t->p->y /= t->p->mass;
	t->p->z /= t->p->mass;

	double mass_weighted_size = 0;
	for(i=0; i<size; i++) {
		double r2 = (ps[i]->x - t->p->x)*(ps[i]->x - t->p->x) + (ps[i]->y - t->p->y)*(ps[i]->y - t->p->y) + (ps[i]->z - t->p->z)*(ps[i]->z - t->p->z);
		mass_weighted_size += r2*ps[i]->mass;
	}
	t->size = sqrt(mass_weighted_size / t->p->mass);

	return(t);
}

void free_tree(tree *t) {
	if(t->dimension != LEAF) {
		free_tree(t->left);
		free_tree(t->right);
		free(t->p);
	}

	free(t);
}
