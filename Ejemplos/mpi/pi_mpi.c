#include <stdio.h>
#include "mpi.h"

#define F(x) (4.0 / (1.0 + x * x ) )  

int main(int argc, char *argv[]){
	int myid, size;	
	long i, m, N, end_range; 
	double  w,x,sum,pi,f,a, sum_shared;

	if(argc < 2){
		printf("Must give a parameter N\n");
		return;
	}

	N = atoi(argv[1]);

	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	m = N/size;

	w = 1.0/N;
	sum = 0.0;
	
	end_range = (myid * m + m);

	if(myid == size -1)
		end_range += (N - end_range);
	
	//printf("Process %d taking range %d - %d\n", myid, (int)(myid * m), (int)end_range);

	for (i = (myid * m); i < end_range ; i++) {
		x = w * (i - 0.5);
		sum = sum + F(x);
	}	

	MPI_Reduce(&sum, &sum_shared, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(myid == 0){
		pi = w * sum_shared;
		printf("Value of pi\n");
		printf("%.16g\n", pi);
	}

	MPI_Finalize();

	return 0;
}  

