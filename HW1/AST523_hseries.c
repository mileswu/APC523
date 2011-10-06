//This program computes the harmonic series up to the nth term and prints
//the error compared to the exact value (entered as an input).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float func(int i);

int main(int argc, char **argv) {
  //check for the correct number of input arguments
  if(argc != 3){
    fprintf(stderr, "There should be 2 arguments, n (integer), and H_{n,e} (floating point number) \n");
    fprintf(stderr, "Usage: %s n H_{n,e} \n", argv[0]);
    return 1;}

  const int imin = 1, imax = atoi(argv[1]);
  const double ans = atof(argv[2]);
  float total = 0;
  int i;
  
  //original version of summation algorithm
  int blocksize = 1;
  int blocks = imax/blocksize;
  int j;
  int sanitycheck = 0;

  for(i = imax; i >= imin + blocksize*blocks -1; i--) {
	  sanitycheck += i;
	  total += func(i);
  }

  for(j=blocks-1; j >= 0; j--) {
	 float temp = 0;
	 int start = imin + blocksize*j;
	 for(i = start; i < start + blocksize; i++) {
	    sanitycheck += i;
		 temp += func(i);
	 }
	 total += temp;
  }
		  
  printf ("error: %e \n", ans - (double) total);
  printf ("sanity check: %d \n", sanitycheck);
  return 0;
}

float func(int i){
  return 1./i;
}
