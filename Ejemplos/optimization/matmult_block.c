#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Uncomment for MKL BLAS 
//#include "mkl.h"

void block_matrixmatrixmultiply(double* a, double* b, double* c, int n);

int index(int i,int j,int n);

int index(int i,int j,int n){
  return i*n + j;
}

int main (int argc, char *argv[]) {
  int	i, j, k;
  double *a,*b,*c;
  int n;
  double sum;

  n = 2000;
  
  a = (double*)malloc(sizeof(double)*n*n);
  b = (double*)malloc(sizeof(double)*n*n);
  c = (double*)malloc(sizeof(double)*n*n);
   
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      a[index(i,j,n)] = i+j;
  
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      b[index(i,j,n)] = i*j;

  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      c[index(i,j,n)] = 0.0;
  
  block_matrixmatrixmultiply(a,b,c,n);

  // Uncomment for MKL_BLAS
  //cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
  //	      n,n,n,1.0,a,n,b,n,1.0,c,n);
  
/*** Print results ***/
printf("******************************************************\n");
printf("Result Matrix:\n");
 sum = 0.0;
 
 for (i=0; i<n; i++)
   sum += c[index(i,i,n)];
	
 printf("%8.2f", sum);
 printf("\n"); 
 printf("******************************************************\n");
 printf ("Done.\n");
}

void block_matrixmatrixmultiply(double* a, double* b, double* c, int n){
  int i,j,k, i0, j0, k0, step;
  
  step = 100;

  for (i0=0; i0 < n; i0+=step) {
    for (j0=0; j0 < n; j0+=step) {          
      for(k0=0; k0 < n; k0+=step) {   
        for(i=i0; i <= fmin(i0 + step - 1, n); i++) {
	  for(j=j0; j <= fmin(j0 + step - 1, n); j++) {
	    for(k=k0; k <= fmin(k0 + step - 1, n); k++) {
		c[index(i,j,n)] += a[index(i,k,n)] * b[index(k,j,n)];
            }
          }
        }
      }
    }
  } 
}
