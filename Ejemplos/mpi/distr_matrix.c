// School on parallel programming, ICTP, Trieste, 2014
// Jeudy Blanco - UCR

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define TAG 100

void printMatrix(int *matrix, int pid, int rows, int cols){
	int i,j,x;
	//printf("This is data from process %d\n", pid);
	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++)
			printf("%d,", matrix[i*rows + j]);
		printf("\n");
	}
	//printf("\n");
}


int main(int argc, char *argv[]){

	int myid, size, i, ii, j, k, received, N, rows;
	
	int *matrix_piece;

	MPI_Status status;
	
	// Some validations, lets stay classy
	if(argc < 2){
		printf("Must give a parameter N\n");
		return;
	}

	N = atoi(argv[1]);

	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Some validations, lets stay classy
	if(N % size != 0){
		if(myid == 0)		
			printf("The Matrix dimension %d must be divisible by the number of processes %d\n", N, size);
		MPI_Finalize();
		return;
	}	
	
	// How many rows should I allocate?	
	rows = N / size;

	// Code to allocate memory for the matrix piece of size rows x N	
	matrix_piece = (int*)malloc(sizeof(int)*rows*N);

	// To make it easy to identify the data from each piece, initialize it with the id of the process
	for(i = 0; i < rows; i++)
		for(j = 0; j < N; j++)
			matrix_piece[i*rows + j] = myid;

	// Print the elements from root elements first
	if(myid == 0){
		printMatrix(matrix_piece, myid, rows, N);
	}
	
	if(myid == 0){
		// Receive data from child processes
		for(ii = 1; ii < size; ii++){		
			MPI_Recv(matrix_piece, rows*N, MPI_INT, ii, TAG, MPI_COMM_WORLD, &status);
			printMatrix(matrix_piece, ii, rows, N);
		}
	}else{
		// Do the transmision of the data to the root process
		MPI_Send(matrix_piece, rows*N, MPI_INT, 0, TAG, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	
	free(matrix_piece);

	return 0;
}
