#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include "particles.h"

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
	for(i=0; i<102400; i++) {
		ps[i].x = gaussian()/3;
		ps[i].y = gaussian()/3;
		ps[i].z = gaussian()/3;
	}
}

void output_image(particle *ps, int num_particles) {
	int width = 102;
	int height = 102;
	int i, j;
	
	double x_min=-2, x_max=2, y_min=-2, y_max = 2;

	int *counters[height];
	for(i=0; i<height; i++) {
		counters[i]=(int *)malloc(sizeof(int) * width);
		for(j=0; j<width; j++)
			counters[i][j] = 0;
	}

	double binsize_x = (x_max - x_min)/((double)width);
	double binsize_y = (y_max - y_min)/((double)height);
	for(i=0; i<num_particles; i++) {
		if(ps[i].x < x_min || ps[i].x > x_max)
			continue;
		if(ps[i].y < y_min || ps[i].y > y_max)
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
			row[j*3] = counters[i][j]*255/max_in_one_bin;
			row[j*3+1] = 0; //g
			row[j*3+2] = 0; //b
		}

		row_pointers[i] = (png_byte *)row;
	}

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
