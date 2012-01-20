#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include "particles.h"
#include "tree.h"

double rand01() {
		return ((double)rand()) / ((double)RAND_MAX);
}

double gaussian() {
	double u1 = rand01();
	double u2 = rand01();
	return (sqrt(-2.0*log(u1)) * cos(2*M_PI*u2));
}

void randomize_particles(particle *ps, int size) {
	int i;
	for(i=0; i<size; i++) {
		ps[i].x = gaussian()/3;
		ps[i].y = gaussian()/3;
		ps[i].z = gaussian()/3;
	}
}


void draw_line(png_byte **row_pointers, int dimension, double line, double linemin, double linemax, double x_min, double x_max, double y_min, double y_max, int width, int height, int color) {
	//Line is the boundary line value
	//Linemin/max is the ones that constrain it in the 2d plane
	double binsize_x = (x_max - x_min)/((double)width);
	double binsize_y = (y_max - y_min)/((double)height);
	
	int i;
	if(dimension == Z) { //can't draw this plane
		return;
	}
	
	if(dimension == X) { //vertical
		int xbin = (line - x_min)/binsize_x;
		if(xbin < 0 || xbin >= width)
			return;
		int ybin_min = (linemin - y_min)/binsize_y;
		int ybin_max = (linemax - y_min)/binsize_y;
		if(ybin_min < 0)
			ybin_min = 0;
		if(ybin_max >= height)
			ybin_max = height-1;

		for(i=ybin_min; i<=ybin_max; i++) {
			row_pointers[i][xbin*3] = 255;
			row_pointers[i][xbin*3+1] = 255;
			row_pointers[i][xbin*3+2] = 255;
		}
	}
	else { //horizontal
		int ybin = (line - y_min)/binsize_y;
		if(ybin < 0 || ybin >= height)
			return;
		int xbin_min = (linemin - x_min)/binsize_x;
		int xbin_max = (linemax - x_min)/binsize_x;
		if(xbin_min < 0)
			xbin_min = 0;
		if(xbin_max >= width)
			xbin_max = width-1;

		for(i=xbin_min; i<=xbin_max; i++) {
			row_pointers[ybin][i*3] = color;
			row_pointers[ybin][i*3+1] = color;
			row_pointers[ybin][i*3+2] = color;
		}
	}
}

void draw_boundaries(png_byte **row_pointers, tree *root, double linex_min, double linex_max, double liney_min, double liney_max, double x_min, double x_max, double y_min, double y_max, int width, int height) {
	int dimension = root->dimension;
	if(dimension == LEAF) return;

	double boundary = root->boundary;

	if(dimension == X || dimension == Y) {
		double linemin = (dimension == X) ? liney_min : linex_min;
		double linemax = (dimension == X) ? liney_max : linex_max;

		int depth = 0;
		tree *t = root->above;
		while(t!=NULL) {
			t = t->above;
			depth++;
		}
		int max_depth = depth;
		t = root;
		while(t!=NULL) {
			t = t->left;
			max_depth++;
		}
		int color = 255*(max_depth - depth)/max_depth;
		
		
		//printf("Boundary[%d-d%d-c%d]: boundary=%f linemin=%f linemax=%f\n", dimension, depth, color, boundary, linemin, linemax);
	
		draw_line(row_pointers, dimension, boundary, linemin, linemax, x_min, x_max, y_min, y_max, width, height, color);
	}

	if(dimension == X) {
		draw_boundaries(row_pointers, root->left, linex_min, boundary, liney_min, liney_max, x_min, x_max, y_min, y_max, width, height);
		draw_boundaries(row_pointers, root->right, boundary, linex_max, liney_min, liney_max, x_min, x_max, y_min, y_max, width, height);
	}
	else if(dimension == Y) {
		draw_boundaries(row_pointers, root->left, linex_min, linex_max, liney_min, boundary, x_min, x_max, y_min, y_max, width, height);
		draw_boundaries(row_pointers, root->right, linex_min, linex_max, boundary, liney_max, x_min, x_max, y_min, y_max, width, height);
	}
	else {
		draw_boundaries(row_pointers, root->left, linex_min, linex_max, liney_min, liney_max, x_min, x_max, y_min, y_max, width, height);
		draw_boundaries(row_pointers, root->right, linex_min, linex_max, liney_min, liney_max, x_min, x_max, y_min, y_max, width, height);
	}

}

void output_image(particle *ps, int num_particles, tree *root) {
	int width = 1000;
	int height = 1000;
	int i, j;
	
	double x_min=-2, x_max=2, y_min=-2, y_max = 2;

	// Histogram the particles
	int *counters[height];
	for(i=0; i<height; i++) {
		counters[i]=(int *)malloc(sizeof(int) * width);
		for(j=0; j<width; j++)
			counters[i][j] = 0;
	}

	double binsize_x = (x_max - x_min)/((double)width);
	double binsize_y = (y_max - y_min)/((double)height);
	for(i=0; i<num_particles; i++) {
		if(ps[i].x < x_min || ps[i].x >= x_max)
			continue;
		if(ps[i].y < y_min || ps[i].y >= y_max)
			continue;
		int xbin = (ps[i].x - x_min)/binsize_x;
		int ybin = (ps[i].y - y_min)/binsize_y;
		counters[ybin][xbin]++;
	}
	
	int max_in_one_bin=0;
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++)
			if(counters[i][j] > max_in_one_bin)
				max_in_one_bin = counters[i][j];
	}

	// Image stuff
	int bit_depth = 8;
	png_byte color_type = PNG_COLOR_TYPE_RGB;

	png_byte *row_pointers[height];

	for(i=0; i<height; i++) {
		char *row = (char *)malloc(sizeof(char)*width*3);
		int j;
		for(j=0; j<width; j++) {
			row[j*3] = 0;
			row[j*3+1] = 0; //g
			row[j*3+2] = 0; //b
		}
		row_pointers[i] = (png_byte *)row;
	}

	// Draw boundaries
	tree *t = root;
	if(root != NULL) {
		draw_boundaries(row_pointers, t, x_min, x_max, y_min, y_max, x_min, x_max, y_min, y_max, width, height);
	}

	// Draw Particles
	for(i=0; i<height; i++) {
		int j;
		for(j=0; j<width; j++) {
			if(counters[i][j] == 0) continue;
			row_pointers[i][j*3] = counters[i][j]*255/max_in_one_bin;
			row_pointers[i][j*3+1] = 0; //g
			row_pointers[i][j*3+2] = 0; //b
		}
	}

	
	// Flip image vertically
	png_byte *row_pointers_flip[height];
	for(i=0; i<height; i++)
		row_pointers_flip[i] = row_pointers[height - i - 1];
	for(i=0; i<height; i++)
		row_pointers[i] = row_pointers_flip[i];

	FILE *fp = fopen("out.png", "wb");
	if(!fp) {
		printf("There was an error. fopen failed\n");
		exit(1);
	}
	
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr) {
		printf("There was an error. png_create_write_struct failed\n");
		exit(1);
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		printf("There was an error. png_create_info_struct failed\n");
		exit(1);
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("There was an error. png_jmpbuf failed\n");
		exit(1);
	}
	png_init_io(png_ptr, fp);
	
	/* write header */
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("There was an error. png_jmpbuf failed\n");
		exit(1);
	}
	png_set_IHDR(png_ptr, info_ptr, width, height,
		bit_depth, color_type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("There was an error. png_jmpbuf failed\n");
		exit(1);
	}
	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("There was an error. png_jmpbuf failed\n");
		exit(1);
	}
	png_write_end(png_ptr, NULL);

	fclose(fp);

	/* Free memory usage */
	for(i=0; i<height; i++) {
		free(row_pointers[i]);
	}

	return;
}
