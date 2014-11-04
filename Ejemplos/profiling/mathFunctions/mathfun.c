#include "mathfun.h"

double sumArray(double* a, int n){
  int i;
  double sum;

  sum = 0.0;
  for(i=0;i<n;i++) {
    sum += a[i];
  }
  
  return sum;
}

double divSumArray(double* a, int n){
  int i;
  double sum;
  
  sum=0.0;
  for(i=0;i<n;i++){
    sum += 1.0/(a[i]+0.000001);
  }
  
  return sum;
}

double sumTwoArrays(double *a, double* b, int n){
  int i;
  double sum;
  
  sum=0.0;
  for(i=0;i<n;i++){
    sum += a[i]*b[i];
  }
  
  return sum;
}
