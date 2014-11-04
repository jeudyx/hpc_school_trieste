#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mathfun.h"

int main(int argc, char *argv[]) {
  int limit = 100000000;
  if (argc != 2) {
    printf("usage: %s N\n",argv[0]);
    return 1;
  }
  else if(atoi(argv[1]) > limit){
    printf("Please specify N to be something lower than %d\n",limit);
    return 1;
  }
  else{
    int i,N;
    double* a,*b,*c,*d;
    double sum,powSum,powSum2,expSum;
    
    N = atoi(argv[1]);
    
    a = (double*)malloc(sizeof(double)*N);
    b = (double*)malloc(sizeof(double)*N);
    c = (double*)malloc(sizeof(double)*N);
    d = (double*)malloc(sizeof(double)*N);

    for(i=0;i<N;i++){
      a[i] = (double) i;
      b[i] = (double) i;
      c[i] = ((double) i)/((double) i + 1.0);
      d[i] = (double) i+3.0;
    }
    
    for(i=0;i<100;i++){
      expSum = divSumArray(d,N);
      sum = sumArray(a,N);
      powSum = sumTwoArrays(b,c,N);
    }
    printf("Sum = %e\n",sum);
    printf("expSum = %e\n",expSum); 
    printf("powSum = %e\n",powSum);
  }
  
  return 0;
}
    
