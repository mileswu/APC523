//This program computes the harmonic series up to the nth term and prints
//the error compared to the exact value (entered as an input).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float func(int i);


float* recursive_add(float *input, int length) {
  int newlength = length/2;
  float *output = (float *)malloc(sizeof(float) * newlength);
  int i;

  // Add each pair and store in the output
  for(i = 0; i < newlength; i++)
    output[i] = input[2*i] + input[2*i+1];
  
  // Deal with the odd length edge case by adding it to the last pair
  if(length%2 == 1)
    output[newlength-1] += input[length-1];
  
  free(input);
  
  // If it is still not a single number, run again
  if(newlength == 1)
    return(output);
  else
    return(recursive_add(output, newlength));
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
  
  // Create an array with all the 1/x terms
  float *input = (float *)malloc(sizeof(float) * (imax-imin+1));
  for(i = imin; i <= imax; i++)
    input[i-imin] = func(i);

  // Run the recursive add function
  float *output = recursive_add(input, imax-imin+1);
  total = output[0];
  free(output);

  printf ("error: %e \n", fabs(ans - (double) total));
  return 0;
}

float func(int i){
  return 1./i;
}
