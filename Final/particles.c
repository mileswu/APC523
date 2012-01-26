#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include <assert.h>
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

void generate_random_point_in_sphere(double *x, double *y, double *z) {
	int i;
	for(i=0; i<10000; i++) {
		*x = rand01();
		if(rand()%2 == 0)
			*x *= -1;
		*y = rand01();
		if(rand()%2 == 0)
			*y *= -1;
		*z = rand01();
		if(rand()%2 == 0)
			*z *= -1;

		if(*x * *x + *y * *y + *z * *z <= 1)
			return;
	}
	printf("Problem generating random point in sphere\n");
	assert(1==0);
}

void generate_two_body(particle *ps, int size) {
	ps[0].v_x = 0;
	ps[0].v_y = 0;
	ps[0].v_z = 0;
	ps[0].x = 0;
	ps[0].y = 0;
	ps[0].z = 0;
	ps[0].mass = 100;

	ps[1].x = 1;
	ps[1].y = 0;
	ps[1].z = 0;
	ps[1].v_x = 0;
	ps[1].v_y = 10;
	ps[1].v_z = 0;
	ps[1].mass = 1;
}

void randomize_uniform_sphere(particle *ps, int size) {
	int i;
	for(i=0; i<size; i++) {
		generate_random_point_in_sphere(&ps[i].x, &ps[i].y, &ps[i].z);
		
		ps[i].v_x = 0;
		ps[i].v_y = 0;
		ps[i].v_z = 0;
		ps[i].id = i;

		double multi=1;
		ps[i].x *= multi;
		ps[i].y *= multi;
		ps[i].z *= multi;

		ps[i].mass = 1;
		ps[i].color = 1;
	}
}

void galaxy(particle *ps, int size, double xoffset, double yoffset, double xvel, int color) {
	int i;
	for(i=0; i<size-14000; i++) {
		/*if(rand()%2)
			ps[i].x = gaussian()/2 - 1;
		else	
			ps[i].x = gaussian()/2 + 1;
		ps[i].y = gaussian()/2;
		ps[i].z = gaussian()/2;*/

		//generate_random_point_in_sphere(&ps[i].x, &ps[i].y, &ps[i].z);

		/*double multi=50;
		ps[i].x *= multi;
		ps[i].y *= multi;
		ps[i].z *= multi;*/
		
		double a = 0.35;
		double windings = 7;
		double t_max = M_PI_2*windings;
		double b = log(2.0/a)/t_max;
		double drift=0.2;

		double t = rand01()*t_max;
		double x = a * exp(b*t) * cos(t);
		x += drift*x*(rand01() - 0.5);
		double y = a * exp(b*t) * sin(t);
		y += drift*y*(rand01() - 0.5);

		if(rand()%2) {
			x *= -1;
			y *= -1;
		}

		ps[i].x = x;
		ps[i].y = y;
		ps[i].z = (rand01()-0.5)*sqrt(x*x + y*y)*drift;
	}
	for(i=size-14000; i<size; i++) {
		ps[i].x = gaussian();
		ps[i].y = gaussian();
		ps[i].z = gaussian();
		//generate_random_point_in_sphere(&ps[i].x, &ps[i].y, &ps[i].z);
		double multi=0.4;
		ps[i].x *= multi;
		ps[i].y *= multi;
		ps[i].z *= multi;
		//ps[i].z = 0; (rand01()-0.5)*sqrt(ps[i].x*ps[i].x + ps[i].y*ps[i].y)*0.3;
	}
	for(i=0; i<size; i++) {
		ps[i].mass = 1;

		double r = sqrt(ps[i].x*ps[i].x + ps[i].y*ps[i].y + ps[i].z*ps[i].z);
		double vconst = 90;//sqrt(size/r);
		double common = vconst/r;

		ps[i].v_x = common*ps[i].y + xvel;
		ps[i].v_y = -common*ps[i].x;
		ps[i].v_z = -common*ps[i].z;
		ps[i].x += xoffset;
		ps[i].y += yoffset;
		ps[i].id = i;
		ps[i].color = color;

	}
}

void hsv(int hue, int *r, int *g, int *b) {
	// Get RGB Color
	double hueprimed = ((double)hue)/60.0;
	double hueprimedmod2 = hueprimed - (double)((int)(hueprimed/2.0))*2.0;
	double x = 1.0 - fabs(hueprimedmod2 - 1);
	if(hueprimed < 1) {
		*r = 255;
		*g = 255*x;
		*b = 0;
	} else if(hueprimed < 2) {
		*r = 255*x;
		*g = 255;
		*b = 0;
	} else if(hueprimed < 3) {
		*r = 0;
		*g = 255;
		*b = 255*x;
	}
	else if(hueprimed < 4) {
		*r = 0;
		*g = 255*x;
		*b = 255;
	} else if(hueprimed < 5) {
		*r = 255*x;
		*g = 0;
		*b = 255;
	} else {
		*r = 255;
		*g = 0;
		*b = 255*x;
	}
}

void draw_line(png_byte **row_pointers, int dimension, double line, double linemin, double linemax, double x_min, double x_max, double y_min, double y_max, int width, int height, int color) {
	int r,g,b;
	hsv(color, &r, &g, &b);

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
			row_pointers[i][xbin*3] = r;
			row_pointers[i][xbin*3+1] = g;
			row_pointers[i][xbin*3+2] = b;
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
			row_pointers[ybin][i*3] = r;
			row_pointers[ybin][i*3+1] = g;
			row_pointers[ybin][i*3+2] = b;
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
		int color = 360*(max_depth - depth)/max_depth;
		
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

void output_image(char *f, particle *ps, int num_particles, tree *root, double range) {
	int width = 1000;
	int height = 1000;
	int i, j;
	
	double x_min=-range, x_max=range, y_min=-range, y_max = range;

	// Histogram the particles
	int *counters[height], *countersaltcolor[height];
	for(i=0; i<height; i++) {
		counters[i]=(int *)malloc(sizeof(int) * width);
		countersaltcolor[i]=(int *)malloc(sizeof(int) * width);
		for(j=0; j<width; j++) {
			counters[i][j] = 0;
			countersaltcolor[i][j] = 0;
		}
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

		if(ps[i].color !=0)
			countersaltcolor[ybin][xbin]++;
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
	max_in_one_bin = 1;
	for(i=0; i<height; i++) {
		int j;
		for(j=0; j<width; j++) {
			if(counters[i][j] == 0) continue;
			//int val = counters[i][j]*255/max_in_one_bin;
			//if(val > 255) val = 255;
			
			int r,g,b,hue;
			hue = 120*((double)(counters[i][j] - countersaltcolor[i][j])/counters[i][j]);
			hsv(hue, &r, &g, &b);

			row_pointers[i][j*3] = r;
			row_pointers[i][j*3+1] = g; //g
			row_pointers[i][j*3+2] = b; //b
		}
	}

	
	// Flip image vertically
	png_byte *row_pointers_flip[height];
	for(i=0; i<height; i++)
		row_pointers_flip[i] = row_pointers[height - i - 1];
	for(i=0; i<height; i++)
		row_pointers[i] = row_pointers_flip[i];

	FILE *fp = fopen(f, "wb");
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
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);

	/* Free memory usage */
	for(i=0; i<height; i++) {


		free(row_pointers[i]);
		free(counters[i]);
		free(countersaltcolor[i]);
	}

	return;
}
