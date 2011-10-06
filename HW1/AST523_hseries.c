//This program computes the harmonic series up to the nth term and prints
//the error compared to the exact value (entered as an input).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float func(int i);

float recurse(int start, int end) {
  /*printf("%d %d\n", start, end);
  if(start != end - 1 && (end-start+1)%2 != 0) {
	  exit(1);
  }*/
  if(start == end - 1) {
	  return (func(start) + func(end));
  }
  else {
	  return ( recurse(start, start + (end-start)/2) + 
	  recurse(start + (end-start)/2 + 1, end));
  }
}

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
  total = recurse(imin, imax);

  printf ("error: %e \n", fabs(ans - (double) total));
  return 0;
}

float func(int i){
  return 1./i;
}
