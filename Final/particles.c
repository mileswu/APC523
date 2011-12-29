#include <stdio.h>
#include <stdlib.h>
#include <png.h>

typedef struct {
	double x;
	double y;
	double z;

	double v_x;
	double v_y;
	double v_z;

	double mass;
} particle;

void output_image(particle *ps);

void main() {
	particle* ps = malloc(sizeof(particle)*1024);
	
	int i;
	for(i=0; i<1024; i++) {
		ps[i].x = ((double)rand()) / ((double)RAND_MAX);
		ps[i].y = ((double)rand()) / ((double)RAND_MAX);
		ps[i].z = ((double)rand()) / ((double)RAND_MAX);
	}

	output_image(ps);
}

void output_image(particle *ps) {
	int width = 1024;
	int height = 1024;
	int bit_depth = 8;
	png_byte color_type = PNG_COLOR_TYPE_RGB;

	png_byte *row_pointers[height];
	int i;
	for(i=0; i<height; i++) {
		char *row = (char *)malloc(sizeof(char)*width*3);
		
		int j;
		for(j=0; j<width; j++) {
			row[j*3] = 255; //r
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
	return;
}
