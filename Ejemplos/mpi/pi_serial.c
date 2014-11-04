#include <stdio.h>

#define F(x) (4.0 / (1.0 + x * x ) )  

int main(int argc, char *argv[]){
 long n , i ; 
 double  w,x,sum,pi,f,a;


if(argc < 2){
	printf("Must give a parameter N\n");
	return;
}

n = atoi(argv[1]);


 w = 1.0/n;
 sum = 0.0;

 for ( i = 1 ; i < n ; i++ ) {
  	x = w * (i - 0.5);
   	sum = sum + F(x);
  } 

 pi = w * sum ;
 printf("Value of pi\n");
 printf("%.16g\n", pi);


 return 0;
}  

