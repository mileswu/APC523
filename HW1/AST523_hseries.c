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
  for (i = imin; i <= imax; i++){
  total += func(i);
  }
  
  printf ("error: %e \n", fabs(ans - (double) total));
  return 0;
}

float func(int i){
  return 1./i;
}
